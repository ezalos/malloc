/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:22:16 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:22:17 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			show_alloc_alloc(void *ptr, size_t size)
{
	print_hex((size_t)ptr);
	write(1, " - ", 3);
	print_hex((size_t)ptr + size);
	write(1, " : ", 3);
	print_dec(size);
	write(1, " octets\n", 8);
}

void			show_alloc_zone_init(t_zone *z)
{
	write(1, "\x1b[38;2;155;155;255m", 19);
	if (HDR_TYPE_LARGE == (z->first_alloc_header.flags & HDR_TYPE))
		write(1, "LARGE : ", 8);
	else if (HDR_TYPE_TINY == (z->first_alloc_header.flags & HDR_TYPE))
		write(1, "TINY : ", 7);
	else if (HDR_TYPE_SMALL == (z->first_alloc_header.flags & HDR_TYPE))
		write(1, "SMALL : ", 8);
	print_hex((size_t)z);
	write(1, " - ", 3);
	print_hex((size_t)z + z->header.size);
	write(1, "\x1b[0m\n", 5);
}

void			show_alloc_zone(t_zone *z)
{
	t_alloc_header		*al;

	show_alloc_zone_init(z);
	al = &z->first_alloc_header;
	if (HDR_TYPE_LARGE == (z->first_alloc_header.flags & HDR_TYPE))
	{
		if (al->flags & HDR_AVAILABLE)
			write(1, "\x1b[38;2;155;255;155m", 19);
		else
			write(1, "\x1b[38;2;255;155;155m", 19);
		show_alloc_alloc((void*)al + sizeof(t_alloc_header),
				z->header.size - sizeof(t_zone));
		write(1, "\x1b[0m", 4);
	}
	else
		while (al)
		{
			if (al->flags & HDR_AVAILABLE)
				write(1, "\x1b[38;2;155;255;155m", 19);
			else
				write(1, "\x1b[38;2;255;155;155m", 19);
			show_alloc_alloc((void*)al + sizeof(t_alloc_header), al->size);
			write(1, "\x1b[0m", 4);
			al = alloc_access_next(al);
		}
}

void			show_alloc_mem(void)
{
	t_zone			*zone;
	size_t			smallest;

	if (static_mem()->is_init != TRUE)
		return ;
	pthread_mutex_lock(&static_mem()->lock);
	smallest = 0;
	zone = next_smallest(smallest);
	if ((size_t)zone == (size_t)-1)
		write(1, "Malloc memory is empty\n: ", 23);
	while ((size_t)zone != (size_t)-1)
	{
		show_alloc_zone(zone);
		zone = next_smallest((size_t)zone);
	}
	pthread_mutex_unlock(&static_mem()->lock);
}
