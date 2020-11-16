/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_unavailable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:19:50 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:19:52 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_rbt		**unavailable_get_tree(void *ptr)
{
	t_infos	*infos;
	size_t	hash;

	infos = static_mem();
	hash = hash_djb2((unsigned char*)&ptr) % UNAVAILABLE_TABLE_SIZE;
	return (&infos->unavailable[hash]);
}

uint8_t		unavailable_exists(void *maybe_alloc_header)
{
	t_rbt	**tree;

	tree = unavailable_get_tree(maybe_alloc_header);
	if (tree_get_recurse_func_ll(*tree, maybe_alloc_header,
		&compare_adresses))
		return (TRUE);
	return (FALSE);
}

int8_t		unavailable_remove(void *maybe_alloc_header)
{
	t_rbt	**tree;
	t_rbt	*node;

	tree = unavailable_get_tree(maybe_alloc_header);
	if (!(node = tree_get_recurse_func_ll(*tree, maybe_alloc_header,
			&compare_adresses)))
	{
		if (PRINT_INVALID_FREE == TRUE)
		{
			write(1, "Error can't find alloc in unavailable tree: ", 44);
			print_hex((size_t)maybe_alloc_header);
			write(1, "\n", 1);
		}
		return (FAILURE);
	}
	*tree = tree_delete_node(node);
	return (SUCCESS);
}

void		unavailable_add(t_alloc_header *alloc_header)
{
	t_rbt	**tree;

	tree = unavailable_get_tree(alloc_header);
	*tree = tree_insert_func_ll(*tree, &alloc_header->rbt,
		(void*)alloc_header, &compare_adresses);
}

long long	compare_adresses(void *content1, void *content2)
{
	return ((long long)(content1 - content2));
}
