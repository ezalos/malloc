/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_get.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:21:31 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:21:32 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

uint8_t				is_large_zone(size_t size)
{
	if (size > static_mem()->small.alloc_size_max)
		return (TRUE);
	return (FALSE);
}

t_alloc_header		*alloc_get_available(size_t size)
{
	t_rbt			**tree;
	t_rbt			*node;
	int				umpteenth_node;

	if (NULL == (tree = available_get_tree_with_memory(size)))
		return (NULL);
	umpteenth_node = 0;
	if (!(node = tree_get_node_th(*tree, &umpteenth_node)))
	{
		write(1, "Error: can't get %dth node of tree: \n", 37);
		print_dec((size_t)umpteenth_node);
		write(1, "\n", 1);
		return (NULL);
	}
	return ((t_alloc_header*)node);
}

t_alloc_header		*alloc_get(size_t size)
{
	t_alloc_header	*alloc;

	size = secure_align_size(size);
	if (TRUE == is_large_zone(size))
	{
		alloc = zone_create_large(size);
		if (NULL == alloc)
			return (NULL);
	}
	else
	{
		alloc = alloc_get_available(size);
		if (NULL == alloc)
			return (NULL);
		available_remove(alloc);
		if (SUCCESS == alloc_split_malloc(alloc, size))
			available_add(alloc_access_next(alloc));
	}
	alloc_set_unavailable(alloc);
	unavailable_add(alloc);
	return (alloc);
}
