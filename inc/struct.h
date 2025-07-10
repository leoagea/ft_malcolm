/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:33:13 by lagea             #+#    #+#             */
/*   Updated: 2025/07/09 15:50:33 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

#define MAC_ADD_STR_LEN 18

#include <netinet/in.h>			// INET_ADDRSTRLEN
#include <linux/if_ether.h>		// ETH_ALEN

typedef int socketfd;
typedef struct ifaddrs net_int;

typedef struct 
{
	u_int8_t  h_dest[ETH_ALEN];		// destination eth addr
	u_int8_t  h_source[ETH_ALEN];	// source ether addr
	u_int16_t h_proto;				// packet type ID field
}	__attribute__((packed)) t_ethhdr;

typedef struct 
{
	u_int16_t htype;			// Hardware type (1 for Ethernet)
	u_int16_t ptype;			// Protocol type (0x0800 for IPv4)
	u_int8_t  hlen;				// Hardware address length (6 for Ethernet)
	u_int8_t  plen;				// Protocol address length (4 for IPv4)
	u_int16_t oper;				// Operation (1 for request, 2 for reply)
	u_int8_t  sha[ETH_ALEN];	// Sender hardware address
	u_int8_t  spa[4];			// Sender protocol address
	u_int8_t  tha[ETH_ALEN];	// Target hardware address
	u_int8_t  tpa[4];			// Target protocol address
} __attribute__((packed)) t_arphdr_payload;

typedef struct 
{
	t_ethhdr ethhdr; // Ethernet header
	t_arphdr_payload arphdr; // ARP header
}	__attribute__((packed)) t_arp_packet;

typedef struct 
{
	char ip[INET_ADDRSTRLEN];
	char mask[INET_ADDRSTRLEN];

	u_int32_t iip;
	u_int32_t imask;

	char mac[18];  // "XX:XX:XX:XX:XX:XX\0"
}	t_host;

typedef struct 
{
	t_host source;
	t_host destination;
	
	t_arp_packet packet;

	socketfd sockfd;

	int ifindex;
	net_int *ifaddr;

	net_int *ifaddr_tmp;
}	t_data;

#endif