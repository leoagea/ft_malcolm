/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:16:15 by lagea             #+#    #+#             */
/*   Updated: 2025/07/09 15:50:51 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static void build_arp_packet(t_data *data)
{
	t_arp_packet *packet = &data->packet;
	ft_memset(packet, 0, sizeof(t_arp_packet));

	// Fill Ethernet header
	ft_memcpy(packet->ethhdr.h_dest, data->destination.mac, ETH_ALEN);
	ft_memcpy(packet->ethhdr.h_source, data->source.mac, ETH_ALEN);
	packet->ethhdr.h_proto = htons(ETH_P_ARP);

	// Fill ARP header
	packet->arphdr.htype = htons(1); // Ethernet
	packet->arphdr.ptype = htons(ETH_P_IP); // IPv4
	packet->arphdr.hlen = ETH_ALEN;
	packet->arphdr.plen = 4;
	packet->arphdr.oper = htons(2); // ARP reply

	// Fill ARP payload
	ft_memcpy(packet->arphdr.sha, data->source.mac, ETH_ALEN); // Sender MAC
	ft_memcpy(packet->arphdr.spa, &data->source.iip, 4);       // Sender IP
	ft_memcpy(packet->arphdr.tha, data->destination.mac, ETH_ALEN); // Target MAC
	ft_memcpy(packet->arphdr.tpa, &data->destination.iip, 4);       // Target IP
}

static void send_arp_reply(t_data *data)
{
	struct sockaddr_ll addr;
	
	ft_memset(&addr, 0, sizeof(addr));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = data->ifindex;
	addr.sll_protocol = htons(ETH_P_ARP);
	addr.sll_halen = ETH_ALEN;

	ft_memcpy(addr.sll_addr, data->destination.mac, ETH_ALEN);

	build_arp_packet(data);
	
	ssize_t bytes_sent = sendto(data->sockfd, &data->packet, sizeof(t_arp_packet), 0, (struct sockaddr *)&addr, sizeof(addr));
	if (bytes_sent < 0) {
		print_errno("sendto");
		return;
	}

	printf("ARP reply sent successfully. Bytes sent: %zd\n", bytes_sent);
}

static void process_arp_packet(t_data *data, const unsigned char *buffer)
{
	struct ethhdr *eth = (struct ethhdr *)buffer;

	if (ntohs(eth->h_proto) == ARPOP_REQUEST) {
		printf("ARP Request intercepted:\n");

		uint32_t target_ip = *((uint32_t *)(buffer + ETH_ALEN + sizeof(struct arphdr) + 6 + 4 + 6));
		printf("Target IP: %s\n", inet_ntoa(*(struct in_addr *)&target_ip));

		if (target_ip == data->destination.iip){
			printf("ARP request for our target IP detected.\n");
			send_arp_reply(data);
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

		printf("Received ARP packet of size: %zd bytes\n", bytes_received);
		process_arp_packet(data, buffer);
	}
	printf("Finished listening for ARP requests.\n");
}