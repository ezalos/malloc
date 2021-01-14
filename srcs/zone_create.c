/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 17:59:00 by ezalos            #+#    #+#             */
/*   Updated: 2021/01/14 12:16:04 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			zone_header_init(t_zone_header *header, t_zone *next,
					size_t size)
{
	header->next_zone = next;
	header->prev_zone = NULL;
	if (header->next_zone)
		header->next_zone->header.prev_zone = (t_zone*)header;
	header->size = size;
}

/*
**	This function create a zone of allocation of size 'zone_size' at the end of
**	the zone linked list.
**	It then adds a new entry available memory index
*/

int8_t			zone_create(t_mem_type *mem_type)
{
	t_zone			*zone;
	uint8_t			flags;

	zone = mmap(NULL,
				mem_type->size,
				PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
	{
		write(1, "Error: can't do mmap of size \n", 31);
		print_dec((size_t)mem_type->size);
		write(1, "\n", 1);
		return (ERROR);
	}
	zone_header_init(&zone->header, mem_type->zone, mem_type->size);
	mem_type->zone = zone;
	flags = HDR_AVAILABLE | HDR_POS_LAST | HDR_POS_FIRST | mem_type->type;
	alloc_header_init(&zone->first_alloc_header, mem_type->size
				- sizeof(t_zone_header) - sizeof(t_alloc_header), 0, flags);
	available_add(&zone->first_alloc_header);
	return (SUCCESS);
}

/*
**	Size must have been aligned with secure_align_size() before
*/

t_alloc_header	*zone_create_large(size_t size)
{
	t_zone			*zone;
	uint8_t			flags;
	size_t			zone_size;

	size = size + sizeof(t_alloc_header) + sizeof(t_zone_header);
	zone_size = (size - 1) + getpagesize()
			- ((size - 1) % getpagesize());
	zone = mmap(NULL, zone_size,
				PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == MAP_FAILED)
	{
		write(1, "Error: can't do mmap of large size \n", 36);
		print_dec((size_t)zone_size);
		write(1, "\n", 1);
		return (NULL);
	}
	zone_header_init(&zone->header, static_mem()->large, zone_size);
	static_mem()->large = zone;
	flags = HDR_AVAILABLE | HDR_POS_LAST | HDR_POS_FIRST | HDR_TYPE_LARGE;
	alloc_header_init(&zone->first_alloc_header, zone_size
		- sizeof(t_alloc_header) - sizeof(t_zone_header), 0, flags);
	return (&zone->first_alloc_header);
}
