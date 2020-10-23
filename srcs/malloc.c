/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 17:00:50 by ezalos            #+#    #+#             */
/*   Updated: 2020/10/23 17:06:22 by arobion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		malloc_exit(void)
{
	t_malloc	*base;
	int8_t			retval;

	retval = SUCCESS;
	base = *static_mem();
	if (ERROR == zone_liberate_all(base->small_zone, base->small_zone_size))
		retval = ERROR;
	if (ERROR == zone_liberate_all(base->tiny_zone, base->tiny_zone_size))
		retval = ERROR;
    if (-1 == munmap((void*)base, sizeof(t_malloc)))
		retval = ERROR;
	return (retval);
}

static int8_t	malloc_init(t_infos *infos)
{
	infos->magic = 0x334C4152; // put magic to recognize init later;
	if ((infos->address_used_space[0] = mmap(0, SIZE_USED_TAB * 16, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (ERROR);
	dprintf(1, "%#zx || %#x ||%#zx\n", (size_t)(infos->address_used_space[0]), SIZE_USED_TAB * 16, (size_t)(infos->address_used_space[0]) + SIZE_USED_TAB *16);
	return (SUCCESS);
// 	t_malloc	*base;
// 
//     base = mmap(NULL, sizeof(t_malloc), PROT_READ | PROT_WRITE,
// 				MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
// 
// 	if (base == MAP_FAILED)
// 		return (ERROR);
// 
// 	*static_mem() = (t_malloc*)base;
// 
// 	base->small_zone_size = ZONE_SIZE;
// 	base->tiny_zone_size = ZONE_SIZE / 4;
// 
// 	if (ERROR == zone_create(&base->small_zone, base->small_zone_size))
// 		return (ERROR);
// 
// 	if (ERROR == zone_create(&(base->tiny_zone), base->tiny_zone_size))
// 		return (ERROR);
// 
// 	return (SUCCESS);
}

void		*our_malloc(size_t size)
{
	static t_infos	infos;
	void			*addr;
	
	if (infos.magic == 0)
		if (malloc_init(&infos) == ERROR)
			return ((void *)ERROR);
	size = malloc_good_size(size);
	addr = get_allocation(&infos, size);
	return (addr);
}
