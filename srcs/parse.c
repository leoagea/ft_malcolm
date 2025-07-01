/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:13:14 by lagea             #+#    #+#             */
/*   Updated: 2025/07/01 14:11:44 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static bool is_valid_mask(const char *mask)
{
	char *buf = mask;
	int prev_oct = 0;

	while (ft_strchr(mask, '.'))
	{
		char *next_dot = ft_strchr(mask, '.');
		if (next_dot == NULL)
			return false;

		*next_dot = '\0';
		int octet = ft_atoi(mask);
		if (octet < 0 || octet > 255 || octet < prev_oct)
			return false;
		prev_oct = octet;

		mask = next_dot + 1;
	}

	return true;
}

static bool is_valid_ip4(const char *ip)
{
	char *buf = ip;
	while (ft_strchr(ip, '.'))
	{
		char *next_dot = ft_strchr(ip, '.');
		if (next_dot == NULL)
			return false;

		*next_dot = '\0';
		int octet = ft_atoi(ip);
		if (octet < 0 || octet > 255)
			return false;

		ip = next_dot + 1;
	}

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
		print_gai_error(status);
		return false;
	}

	struct addrinfo *p;
	for (p = res; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			freeaddrinfo(res);
			return is_valid_ip4(ip);
		}
	}

}

ssize_t parse_arg(int ac, char **av, t_data *data)
{
	ft_strlcpy(data->source.ip, av[1], INET_ADDRSTRLEN);
	ft_strlcpy(data->source.mask, av[2], INET_ADDRSTRLEN);

	ft_strlcpy(data->destination.ip, av[3], INET_ADDRSTRLEN);
	ft_strlcpy(data->destination.mask, av[4], INET_ADDRSTRLEN);
	
	debug_print_source_dest_ip(data);
	
	if (!check_ip(data->source.ip) || !check_ip(data->destination.ip))
		return -1;
	if (!is_valid_mask(data->source.mask) || !is_valid_mask(data->destination.mask))
		return -1;

	return EXIT_SUCCESS;
}