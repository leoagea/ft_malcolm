/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:02:19 by lagea             #+#    #+#             */
/*   Updated: 2025/07/01 14:02:43 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

void debug_print_source_dest_ip(t_data *data)
{
	printf("Source IP: %s\n", data->source.ip);
	printf("Source Mask: %s\n", data->source.mask);
	printf("Destination IP: %s\n", data->destination.ip);
	printf("Destination Mask: %s\n", data->destination.mask);
}