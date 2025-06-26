/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:34:03 by lagea             #+#    #+#             */
/*   Updated: 2025/06/26 16:40:09 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

t_data	*g_data = NULL;

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	g_data = ft_calloc(sizeof(t_data), 1);
	if (!g_data)
		return (EXIT_FAILURE);


	free_data(g_data);
	return (EXIT_SUCCESS);
}