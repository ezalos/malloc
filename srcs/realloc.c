/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:22:28 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:22:29 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static void		*realloc_cant(t_alloc_header *alloc_header, size_t size)
{
	void			*new_ptr;
	size_t			n;

	pthread_mutex_unlock(&static_mem()->lock);
	new_ptr = our_malloc(size);
	if (new_ptr)
	{
		n = alloc_header->size <= size ? alloc_header->size : size;
		ft_memncpy(new_ptr, (void*)alloc_header + sizeof(t_alloc_header), n);
	}
	our_free((void*)alloc_header + sizeof(t_alloc_header));
	pthread_mutex_lock(&static_mem()->lock);
	return (new_ptr);
}

static void		*realloc_smaller(t_alloc_header *alloc_header, size_t size)
{
	if (SUCCESS == alloc_split(alloc_header, size))
	{
		unavailable_remove(alloc_header);
		unavailable_add(alloc_header);
		defragment(alloc_access_next(alloc_header));
		available_add(alloc_access_next(alloc_header));
	}
	return ((void*)alloc_header + sizeof(t_alloc_header));
}

static void		*realloc_can(t_alloc_header *alloc_header, size_t size)
{
	void			*mem;

	if (NULL == alloc_join_realloc(alloc_header, size))
	{
		mem = realloc_cant(alloc_header, size);
	}
	else
	{
		unavailable_remove(alloc_header);
		unavailable_add(alloc_header);
		mem = realloc_smaller(alloc_header, size);
	}
	return (mem);
}

void			*realloc_cases(t_alloc_header *alloc_header, size_t size)
{
	t_mem_type		*mem_type;
	void			*mem;

	pthread_mutex_lock(&static_mem()->lock);
	mem = NULL;
	if (TRUE == unavailable_exists((void*)alloc_header))
	{
		size = secure_align_size(size);
		mem_type = mem_type_get_from_size(size);
		if ((!mem_type && !(alloc_header->flags & HDR_TYPE_LARGE))
		|| (mem_type && 0 == (alloc_header->flags & mem_type->type)))
			mem = realloc_cant(alloc_header, size);
		else if (size <= alloc_header->size)
			mem = realloc_smaller(alloc_header, size);
		else if (alloc_header->flags & HDR_TYPE_LARGE)
			mem = realloc_cant(alloc_header, size);
		else
			mem = realloc_can(alloc_header, size);
	}
	pthread_mutex_unlock(&static_mem()->lock);
	return (mem);
}

void			*our_realloc(void *ptr, size_t size)
{
	t_alloc_header	*alloc_header;
	void			*mem;

	if (ptr == NULL || size == 0)
	{
		if (size == 0)
			our_free(ptr);
		mem = our_malloc(size);
		ft_bzero(mem, secure_align_size(size));
		return (mem);
	}
	if (static_mem()->is_init != TRUE)
		malloc_init();
	alloc_header = (t_alloc_header*)((uint8_t*)ptr - sizeof(t_alloc_header));
	mem = realloc_cases(alloc_header, size);
	return (mem);
}
