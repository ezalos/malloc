/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_sizes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:19:38 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:19:39 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

size_t			align_size(uint8_t type_flag, size_t size)
{
	size = size - 1;
	if (type_flag & HDR_TYPE_TINY)
		size = ((size >> RES_TINY_SHIFT) << RES_TINY_SHIFT) + RES_TINY;
	if (type_flag & HDR_TYPE_SMALL)
		size = ((size >> RES_SMALL_SHIFT) << RES_SMALL_SHIFT) + RES_SMALL;
	if (type_flag & HDR_TYPE_LARGE)
		size = ((size >> RES_LARGE_SHIFT) << RES_LARGE_SHIFT) + RES_LARGE;
	return (size);
}

size_t			secure_align_size(size_t size)
{
	t_infos			*base;
	size_t			aligned_size;

	base = static_mem();
	if (size < RES_TINY)
		size = RES_TINY;
	aligned_size = align_size(HDR_TYPE_TINY, size);
	if (aligned_size <= base->tiny.alloc_size_max)
		return (aligned_size);
	aligned_size = align_size(HDR_TYPE_SMALL, size);
	if (aligned_size <= base->small.alloc_size_max)
		return (aligned_size);
	aligned_size = align_size(HDR_TYPE_LARGE, size);
	return (aligned_size);
}
