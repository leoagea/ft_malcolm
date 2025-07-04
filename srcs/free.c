/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:36:00 by lagea             #+#    #+#             */
/*   Updated: 2025/06/26 18:10:58 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static void free_pointer(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	
	if (g_data->sockfd > 0){
		if (close(g_data->sockfd) < 0)
			print_errno("close");
		g_data->sockfd = -1;
	}

	free_pointer((void **)&data);
	data = NULL;
}