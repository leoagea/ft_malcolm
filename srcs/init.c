/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:34:42 by lagea             #+#    #+#             */
/*   Updated: 2025/07/11 16:18:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

ssize_t init_socket(t_data *data)
{	
	data->sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (data->sockfd < 0)
		return (print_errno("socket"), -1);
	
	if (setsockopt(data->sockfd, SOL_SOCKET, SO_BINDTODEVICE, data->ifaddr->ifa_name, ft_strlen(data->ifaddr->ifa_name)) < 0)
		return (print_errno("setsockopt"), -1);

	return EXIT_SUCCESS;
}

ssize_t get_bind_interface(t_data *data)
{
	struct ifaddrs *ifaddr, *ifa, *first_ifa = NULL;
	uint32_t ip, mask;
	bool first = true;
	char buf[BUFFER_SIZE] = {0};
	
	if (getifaddrs(&ifaddr) == -1)
		return (print_errno("getifaddrs"), -1);
	
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
		if (!(ifa->ifa_flags & IFF_UP) || ifa->ifa_flags & IFF_LOOPBACK)
			continue;
			
		int family = ifa->ifa_addr->sa_family;
		if (family != AF_INET)
			continue;

		ip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
		mask = ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr;
		
		if (mask == 0 || mask == 0xFFFFFFFF)
			continue;
		
		if (first){
			first_ifa = ifa;
			first = false;
		}

		if ((ip & mask) == (data->source.iip & data->source.imask))
			break;
	}

	if (ifa == NULL)
		data->ifaddr = first_ifa;
	else
		data->ifaddr = ifa;

	data->ifaddr_tmp = ifaddr;
	snprintf(buf, BUFFER_SIZE, "Using interface: %s\n", data->ifaddr->ifa_name);
	_(STDOUT_FILENO, buf);
	
	if ((data->ifindex = if_nametoindex(data->ifaddr->ifa_name)) == 0){
		print_errno("if_nametoindex");
		return -1;
	}
	
	return EXIT_SUCCESS;
}