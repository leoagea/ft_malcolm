/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:33:13 by lagea             #+#    #+#             */
/*   Updated: 2025/06/26 19:05:02 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __STRUCT_H__
# define __STRUCT_H__

#include <netinet/in.h>			// INET_ADDRSTRLEN

typedef int socketfd;
typedef struct ifaddrs net_int;

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
	
	socketfd sockfd;
	net_int *ifaddr;
}	t_data;

#endif