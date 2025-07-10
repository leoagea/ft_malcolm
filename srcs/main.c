/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:34:03 by lagea             #+#    #+#             */
/*   Updated: 2025/07/09 15:51:10 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

t_data	*g_data = NULL;

static void signal_handler(int sig)
{
	if (sig == SIGINT || sig == SIGTERM)
	{
		if (g_data)
			free_data(g_data);
		_(STDOUT_FILENO, "Exiting...\n");
		exit(EXIT_SUCCESS);
	}
}

static void init_signals(void)
{
	size_t len = 0;
	char buf[BUF_SIZE] = {0};
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		len = snprintf(buf, BUF_SIZE,"sigaction: %s\n",strerror(errno));
	if (sigaction(SIGTERM, &sa, NULL) == -1)
		len += snprintf(buf + len , BUF_SIZE - len,"sigaction: %s\n",strerror(errno));
	if (len > 0)
		exit_error(buf);
}

int main(int ac, char **av)
{
	(void)av;
	(void)ac;
	// Commented out the usage check for now, for testing purposes.
	// if (ac < 5)
	// 	return (usage(), EXIT_SUCCESS);
	
	if (getuid() != 0)
		exit_error("ft_malcolm: This program must be run as root to enable SOCK_RAW creation.\n");

	g_data = ft_calloc(sizeof(t_data), 1);
	if (!g_data)
		return (EXIT_FAILURE);
	
	init_signals();

	if (parse_arg(av, g_data) == -1)
		return (free_data(g_data), EXIT_FAILURE);

	if (get_bind_interface(g_data) == -1 || init_socket(g_data) == -1 )
		return (free_data(g_data), EXIT_FAILURE);
	
	listen_arp_request(g_data);

	free_data(g_data);
	return (EXIT_SUCCESS);
}