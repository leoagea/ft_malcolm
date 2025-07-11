/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:29:54 by lagea             #+#    #+#             */
/*   Updated: 2025/07/10 16:36:21 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FT_MALCOLM_H__
#define __FT_MALCOLM_H__

/*#############################################################################
# Includes
#############################################################################*/

#include "../libft/libft.h"
#include "struct.h"

#include <stdbool.h>			// bool type
#include <signal.h>				// sigaction
#include <sys/socket.h>			// socket
#include <netinet/ip.h>			// IPPROTO_TCP, IP_HDRINCL
#include <ifaddrs.h>			// getifaddrs
#include <net/if.h>				// IFF_UP, IFF_LOOPBACK. if_nametoindex
#include <arpa/inet.h>			// inet_ntoa, inet_ntop  ???TODO: remote unauthorized includes
#include <netdb.h>				// getaddrinfo
#include <sys/types.h>			// ssize_t
#include <net/if_arp.h>			// ARP protocol definitions
#include <linux/if_packet.h>	// sockaddr_ll
#include <linux/if_ether.h>		// ETH_P_ARP

/*#############################################################################
# Define Variables
#############################################################################*/

#define BUF_SIZE 2048
#define ARPOP_REQUEST 1
#define ARPOP_REPLY 2
#define _(fd, msg)			  do { ssize_t __unused_result = write(fd, msg, ft_strlen(msg)); (void)__unused_result; } while(0)

/*#############################################################################
# Typedef Variables
#############################################################################*/

/*#############################################################################
# Global Variables
#############################################################################*/

extern t_data		   *g_data;

/*#############################################################################
# Init.c
#############################################################################*/

ssize_t init_socket(t_data *data);
ssize_t get_bind_interface(t_data *data);

/*#############################################################################
# Parse.c
#############################################################################*/

ssize_t parse_arg(char **av, t_data *data);

/*#############################################################################
# Request.c
#############################################################################*/

void listen_arp_request(t_data *data);

/*#############################################################################
# Utils.c
#############################################################################*/

void usage(void);
void exit_error(const char *msg);
void print_errno(const char *func_name);
void print_gai_error(int status);

/*#############################################################################
# Free.c
#############################################################################*/

void	free_data(t_data *data);

/*#############################################################################
# Debug.c
#############################################################################*/

void debug_print_source_dest_ip(t_data *data);
void debug_print(const char *msg);

#endif