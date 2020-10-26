/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 20:59:37 by ezalos            #+#    #+#             */
/*   Updated: 2020/10/26 11:16:42 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tree.h"

#define NB_OF_SPACE		15

size_t		ft_nb_len(intmax_t n, size_t base)
{
	size_t		size;
	uintmax_t	nb;

	if (n == 0)
		return (1);
	size = 0;
	if (n < 0)
	{
		size++;
		nb = -n;
	}
	else
		nb = n;
	if (base == 0)
		base = 10;
	while (nb != 0)
	{
		nb /= base;
		size++;
	}
	return (size);
}

void	padding_before(t_rbt *node, size_t space)
{
	t_rbt		*parent;
	size_t		i;

	i = 0;
	parent = tree_parent(node);
	while (i < space)
	{
		printf(" ");
		i++;
	}
	if (parent != NULL)
		(parent->left == node) ? printf("╰─ ") : printf("╭─ ");
}

void	padding_after(t_rbt *node)
{
	size_t		i;
	size_t		padding;

	if (node->left != NULL || node->right != NULL)
	{
		padding = NB_OF_SPACE - ft_nb_len((size_t)node->content, 16) - 4;
		padding = NB_OF_SPACE - 11 - 4;
		i = 0;
		while (i < padding)
		{
			printf("─");
			i++;
		}
		if (tree_root(node) == node)
			printf("───");
		printf("┤");
	}
	printf("\n");
}

void	tree_print_elem(t_rbt *node)
{
	void	*content;

	content = (node->content);
	if (node->color == RED)
		printf("\033[31m");
	else
		printf("\033[34m");
	printf("%p \033[00m", content);
}

void	tree_print(t_rbt *node, size_t deep)
{
	if (node == NULL)
	{
		return ;
	}
	tree_print(node->right, deep + 1);
	padding_before(node, (deep * NB_OF_SPACE));
	tree_print_elem(node);
	padding_after(node);
	tree_print(node->left, deep + 1);
}
