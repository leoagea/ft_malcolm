/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:02:19 by lagea             #+#    #+#             */
/*   Updated: 2025/07/14 11:30:09 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"
#include <stdio.h>

void debug_print_source_dest_ip(t_data *data)
{
	printf("Source IP: %s\n", data->source.ip);
	printf("Source IP (integer): %u\n", data->source.iip);
	printf("Source MAC: %s\n", data->source.mac);
	printf("Destination IP: %s\n", data->destination.ip);
	printf("Destination IP (integer): %u\n", data->destination.iip);
	printf("Destination MAC: %s\n", data->destination.mac);
}

void debug_print(const char *msg)
{
	if (msg){
		printf("DEBUG: %s\n", msg);
	}
	else{
		printf("DEBUG: NULL message\n");
	}
}