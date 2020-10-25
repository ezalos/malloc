/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_insert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 20:56:28 by ezalos            #+#    #+#             */
/*   Updated: 2020/10/23 12:56:11 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"

t_rbt		*tree_insert_func(t_rbt *root, t_rbt *new_node, void *content,
			t_rbt_compare *func)
{
	t_rbt	*n;

	n = tree_new_node(content, new_node);
	tree_insert_recurse_func(root, n, func);
	tree_insert_repair(n);
	root = tree_root(n);
	return (root);
}

t_rbt		*tree_insert_func_ll(t_rbt *root, t_rbt *new_node, void *content,
			t_rbt_compare_long_long *func)
{
	t_rbt	*n;

	n = tree_new_node(content, new_node);
	tree_insert_recurse_func_ll(root, n, func);
	tree_insert_repair(n);
	root = tree_root(n);
	return (root);
}