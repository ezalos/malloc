/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:19:08 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:19:09 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	*our_calloc(size_t count, size_t size)
{
	size_t	total_size;
	size_t	i;
	void	*ptr;

	if (size == 0 || count == 0)
		return (our_malloc(0));
	if (static_mem()->is_init != TRUE)
		malloc_init();
	total_size = 0;
	i = 0;
	while (i < count)
	{
		if (total_size + size < total_size)
			return (NULL);
		total_size += size;
		i++;
	}
	if (secure_align_size(total_size) < total_size)
		return (NULL);
	ptr = our_malloc(total_size);
	if (ptr)
		ft_bzero(ptr, secure_align_size(total_size));
	return (ptr);
}
