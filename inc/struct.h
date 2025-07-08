/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:33:13 by lagea             #+#    #+#             */
/*   Updated: 2025/07/08 16:52:40 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

#define MAC_ADD_LEN 6
#define MAC_ADD_STR_LEN 18

#include <netinet/in.h>			// INET_ADDRSTRLEN

typedef int socketfd;
typedef struct ifaddrs net_int;

typedef struct 
{
	u_int16_t htype;		// Hardware type
	u_int16_t ptype;		// Protocol type
	u_int8_t hlen;			// Hardware address length
	u_int8_t plen;			// Protocol address length
	u_int16_t oper;			// Operation code
	u_int8_t shard[MAC_ADD_LEN];	// Sender hardware address
	u_int32_t sip;			// Sender IP address
	u_int8_t thard[MAC_ADD_LEN];	// Target hardware address
	u_int32_t tip;			// Target IP address
}	t_arp_packet;

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
	net_int *ifaddr;
}	t_data;

#endif