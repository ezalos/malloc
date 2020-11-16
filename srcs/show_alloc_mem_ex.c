/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_ex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:22:08 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:22:09 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			print_alloc_header(t_alloc_header *alloc, size_t *total_octet)
{
	size_t				octet;

	octet = 0;
	while (octet < sizeof(*alloc))
	{
		if (0 == *total_octet % PRINT_LINE_SIZE)
		{
			putstr("\x1b[0m");
			print_hex((size_t)(uint8_t*)alloc + octet);
			putstr(": ");
		}
		mem_put_color(alloc, -octet - 1, TRUE);
		print_hex_octet(*((uint8_t*)alloc + octet));
		octet++;
		(*total_octet)++;
		if (0 == *total_octet % PRINT_LINE_SIZE)
			putstr("\x1b[0m\n");
	}
}

void			print_alloc_memory(t_alloc_header *alloc, size_t *total_octet,
				size_t alloc_nb)
{
	size_t				octet;

	octet = 0;
	while (octet < (size_t)(alloc->size))
	{
		if (0 == *total_octet % PRINT_LINE_SIZE)
		{
			putstr("\x1b[0m");
			print_hex((size_t)(uint8_t*)alloc + sizeof(*alloc) + octet);
			putstr(": ");
			mem_put_color(alloc, alloc_nb, FALSE);
		}
		print_hex_octet(*((uint8_t*)alloc + sizeof(*alloc) + octet));
		octet++;
		(*total_octet)++;
		if (0 == *total_octet % PRINT_LINE_SIZE)
		{
			putstr("\x1b[0m\n");
		}
	}
}

void			show_alloc_mem_ex(void)
{
	t_infos				*base;

	if (static_mem()->is_init != TRUE)
		return ;
	pthread_mutex_lock(&static_mem()->lock);
	base = static_mem();
	putstr("\x1b[38;2;255;105;255m");
	putstr("ZONE TYPE TINY\n");
	print_zone_type(base->tiny.zone);
	if (PRINT_HEX_DEMO == FALSE)
	{
		putstr("\x1b[38;2;255;105;255m");
		putstr("===============\n");
		putstr("ZONE TYPE SMALL\n");
		print_zone_type(base->small.zone);
		putstr("\x1b[38;2;255;105;255m");
		putstr("===============\n");
		putstr("ZONE TYPE LARGE\n");
		print_zone_type(base->large);
	}
	putstr("\x1b[0m");
	pthread_mutex_unlock(&static_mem()->lock);
}
