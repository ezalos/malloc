/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 16:56:21 by arobion           #+#    #+#             */
/*   Updated: 2020/10/23 17:09:01 by arobion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t			create_zone(t_infos *infos)
{
	void		*ret;

	if ((ret = mmap(0, TINY_ZONE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (ERROR);
	dprintf(1, "MMAP : %#zx\n", (size_t)ret);
	infos->pool_tiny_free.address_start = ret;
	infos->pool_tiny_free.size = TINY_ZONE_SIZE - sizeof(t_zone);
	return (SUCCESS);
}

t_used_space	*get_used_tab(t_infos *infos, size_t index)
{
	int tab_num;

	tab_num = index / SIZE_USED_TAB;
	if (tab_num > 128)
		return (NULL);
	if (infos->address_used_space[tab_num] == NULL)
		if ((infos->address_used_space[tab_num] = mmap(0, SIZE_USED_TAB * 16, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
// 	dprintf(1,"%d || %d\n", tab_num);
	return (infos->address_used_space[tab_num]);
}

static void		*create_allocation(t_infos *infos, t_mem_pool *pool, size_t size)
{
	t_used_space	*tab;
	uint32_t		index;
	tab = get_used_tab(infos, infos->index_used_tab);
	index = infos->index_used_tab % SIZE_USED_TAB;

	// met a jour l'used tab
	dprintf(1, "%d || %#zx\n", infos->index_used_tab, (size_t)(&(infos->address_used_space[infos->index_used_tab])));
	tab[index].address = pool->address_start + sizeof(t_header);
	// remplit le header d'allocation
	((t_header *)(pool->address_start))->index = infos->index_used_tab;
	// met a jour le pool
	pool->size -= (size + sizeof(t_header));
	pool->address_start = (void *)((size_t)(pool->address_start) + (size + sizeof(t_header)));
	infos->index_used_tab += 1;
	return (tab[index].address);
}

static void		*get_memory_chunck(t_infos *infos, size_t size)
{
	t_mem_pool		*pool;
// 	t_used_space	*tab;

	pool = &(infos->pool_tiny_free);
	if (pool->address_start == NULL) // check si pool existe
		if (create_zone(infos) == ERROR)
			return (NULL);
	if (pool->size - sizeof(t_header) < size) // check si il y a assez de place dans le pool
		if (create_zone(infos) == ERROR) // TODO : defrag
			return (NULL);
	if (pool->size - sizeof(t_header) < size) // check si il y a assez de place meme apres la defrag, tient compte du header d'alloc
		if (create_zone(infos) == ERROR) // used when defragwill be done
			return (NULL);

	return (create_allocation(infos, pool, size)); // ecrit dans le tableau au bon index + met a jour les infos et le header d'alloc
// 	infos->index_used_tab += 1;

// 	tab = infos->address_used_space; // recupere le tableau
// 	return ((tab[infos->index_used_tab - 1]).address); // renvoie l'adresse memoire
}

static void		*reuse_freed_adress(t_infos *infos, size_t index_size, size_t size)
{
	(void)infos;
	(void)size;
	(void)index_size;
	return (NULL);
}

void			*get_allocation(t_infos *infos, size_t size)
{
// 	t_free_space	*node;
	uint32_t	index_size;

// 	node = infos->tab_free_spaces;
	index_size = size / 16;
	if ((infos->tab_free_spaces[index_size]) == NULL)
		return (get_memory_chunck(infos, size));
	else
		return (reuse_freed_adress(infos, index_size, size));
}
