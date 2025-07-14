/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:13:14 by lagea             #+#    #+#             */
/*   Updated: 2025/07/14 11:09:34 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static bool is_valid_mac_address(const char *mac)
{
	if (ft_strlen(mac) != MAC_ADD_STR_LEN - 1)
		return false;

	for (size_t i = 0; i < MAC_ADD_STR_LEN - 1; i++)
	{
		if ((i + 1) % 3 == 0)
		{
			if (mac[i] != ':')
				return print_mac_addr_error(mac) ,false;
		}
		else if (!ft_isxdigit(mac[i]))
			return print_mac_addr_error(mac) ,false;
	}

	return true;
}

static bool is_valid_ip4(const char *ip)
{
	const char *buf = ip;

	while (ft_strchr(buf, '.'))
	{
		char *next_dot = ft_strchr(buf, '.');
		if (next_dot == NULL){
			free((void *)ip);
			return false;
		}

		*next_dot = '\0';
		int octet = ft_atoi(buf);
		if (octet < 0 || octet > 255){
			free((void *)ip);
			return false;
		}

		buf = next_dot + 1;
	}

	free((void *)ip);
	return true;
}

static bool check_ip(const char *ip)
{
	size_t status = 0;
	struct addrinfo hints, *res;
	
	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_CANONNAME;
	
	status = getaddrinfo(ip, NULL, &hints, &res);
	if (status != 0){
		print_gai_error(ip, status);
		free((void *)ip);
		return false;
	}

	struct addrinfo *p;
	for (p = res; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			freeaddrinfo(res);
			return is_valid_ip4(ip);
		}
	}
	freeaddrinfo(res);
	free((void *)ip);
	return false;
}

ssize_t parse_arg(char **av, t_data *data)
{
	if (!av[1] || !av[2] || !av[3] || !av[4])
		return -1;

	ft_strlcpy(data->source.ip, av[1], INET_ADDRSTRLEN);
	ft_strlcpy(data->source.mac, av[2], MAC_ADD_STR_LEN);

	ft_strlcpy(data->destination.ip, av[3], INET_ADDRSTRLEN);
	ft_strlcpy(data->destination.mac, av[4], MAC_ADD_STR_LEN);
	
	if (!check_ip(ft_strdup(data->source.ip)) || !check_ip(ft_strdup(data->destination.ip)))
		return -1;
	
	if (!is_valid_mac_address(data->source.mac) || !is_valid_mac_address(data->destination.mac))
		return -1;
	
	if (inet_pton(AF_INET, data->source.ip, &data->source.iip) < 0 || inet_pton(AF_INET, data->destination.ip, &data->destination.iip) < 0){
		print_errno("inet_pton");
		return -1;
	}

	return EXIT_SUCCESS;
}