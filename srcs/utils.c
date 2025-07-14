/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:56:55 by lagea             #+#    #+#             */
/*   Updated: 2025/07/14 11:04:49 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

void usage()
{
	char buf[BUF_SIZE];

	snprintf(buf, BUF_SIZE, "Usage: ./ft_malcolm [source ip] [source mac address] [destination ip] [destination mac address]\n");
	_(STDOUT_FILENO, buf);
}

void exit_error(const char *msg)
{
	_(STDERR_FILENO, msg);
	exit(EXIT_FAILURE);
}

void print_errno(const char *func_name)
{
	char buf[BUF_SIZE];
	snprintf(buf, BUF_SIZE, "%s: %s\n", func_name, strerror(errno));
	_(STDERR_FILENO, buf);
}

void print_gai_error(const char *ip, int status)
{
	char buf[BUF_SIZE];
	snprintf(buf, BUF_SIZE, "ft_malcolm: %s: (%s)\n", gai_strerror(status), ip);
	_(STDERR_FILENO, buf);
}

void print_mac_addr_error(const char *mac)
{
	char buf[BUF_SIZE];
	snprintf(buf, BUF_SIZE, "ft_malcolm: Invalid mac address : (%s)\n", mac);
	_(STDERR_FILENO, buf);
}
