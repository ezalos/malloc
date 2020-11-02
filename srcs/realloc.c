/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/26 20:19:47 by rkirszba          #+#    #+#             */
/*   Updated: 2020/11/02 11:01:42 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "head.h"

void    ft_memncpy(void* dest, const void* src, size_t n)
{
    size_t i;

    i = 0;
    while (i < n)
    {
        ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
        i++;
    }
}

static void    *realloc_cant(t_alloc_header *alloc_header, size_t size)
{
    void            *new_ptr;
    
    new_ptr = our_malloc(size);
    if (new_ptr)
        ft_memncpy(new_ptr, (void*)alloc_header + sizeof(t_alloc_header),
        alloc_header->size);
    our_free((void*)alloc_header + sizeof(t_alloc_header));
    return new_ptr;
}

static void    *realloc_smaller(t_alloc_header *alloc_header, size_t size)
{
    if (SUCCESS == alloc_split(alloc_header, size))
	{
		unavailable_remove(alloc_header);
		alloc_set_unavailable(alloc_header);	/*
												** only because it is set to
												** available in unavailable remove
												*/
		unavailable_add(alloc_header);
		defragment(alloc_access_next(alloc_header));
        available_add(alloc_access_next(alloc_header));
	}
    return ((void*)alloc_header + sizeof(t_alloc_header));
}

static void    *realloc_can(t_alloc_header *alloc_header, size_t size)
{
    if (NULL == alloc_join_realloc(alloc_header, size))
        return (realloc_cant(alloc_header, size));
	unavailable_remove(alloc_header);
	alloc_set_unavailable(alloc_header);	/*
											** only because it is set to
											** available in unavailable remove
											*/
	unavailable_add(alloc_header);
	return (realloc_smaller(alloc_header, size));
}


void	*our_realloc(void *ptr, size_t size)
{
    t_alloc_header  *alloc_header;
	t_mem_type		*mem_type;

    if (ptr == NULL || size == 0)
    {
        if (size == 0)
            our_free(ptr);
        return (our_malloc(size));
    }
    if (static_mem()->is_init != TRUE)
		return (NULL);
    alloc_header = ptr - sizeof(t_alloc_header);
    if (unavailable_exists((void*)alloc_header) == FALSE)
        return (NULL);
    size = secure_align_size(size);
	mem_type = mem_type_get_from_size(size);
    if (size <= alloc_header->size)
        return (realloc_smaller(alloc_header, size));
    else if (alloc_header->flags & HDR_TYPE_LARGE)
        return (realloc_cant(alloc_header, size));
    else if (!mem_type || 0 == (alloc_header->flags
    & mem_type->type))
        return (realloc_cant(alloc_header, size));
    else
        return (realloc_can(alloc_header, size));
}