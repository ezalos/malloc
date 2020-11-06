/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 17:00:50 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/06 11:58:59 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		malloc_exit(void)
{
	t_infos			*base;
	int8_t			retval;

	retval = SUCCESS;
	base = static_mem();
	if (ERROR == zone_liberate_all(base->small.zone))
		retval = ERROR;
	if (ERROR == zone_liberate_all(base->tiny.zone))
		retval = ERROR;
	if (ERROR == zone_liberate_all(base->large))
		retval = ERROR;
	base->is_init = FALSE;
	pthread_mutex_destroy(&static_mem()->lock);
	return (retval);
}

void		malloc_init(void)
{
	t_infos	*base;

	base = static_mem();

	mem_type_init(&base->tiny, ZONE_TINY);
	mem_type_init(&base->small, ZONE_SMALL);
	if (pthread_mutex_init(&base->lock, NULL) != 0)
		printf("\n mutex init failed\n");

	base->is_init = TRUE;
}

void		*our_malloc(size_t size)
{
	void	*mem;

	if (static_mem()->is_init != TRUE)
		malloc_init();

	pthread_mutex_lock(&static_mem()->lock);

	mem = alloc_get(size);
	if (mem)
	{
		// hamming_do(mem + sizeof(t_rbt),
		// 		sizeof(t_alloc_header) - sizeof(t_rbt),
		// 		(uint8_t*)&((t_alloc_header*)mem)->parity_bit);
		mem = mem + sizeof(t_alloc_header);
	}
	pthread_mutex_unlock(&static_mem()->lock);
	return (mem);
}
