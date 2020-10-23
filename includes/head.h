/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2020/10/22 18:55:55 by arobion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include <sys/mman.h>
# include <stddef.h>
# include <stddef.h>
# include <inttypes.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

# define ZONE_SIZE 4096
# define SIZE_STATIC 4096

# define ERROR		-1
# define FAILURE	-1
# define SUCCESS	0

# define TRUE		1
# define FALSE		0

# define ZONE_TINY	1
# define ZONE_SMALL	2
# define ZONE_LARGE	3

# define PRINT_LINE_SIZE	32

# define TINY_ZONE_SIZE 2 * 1024 * 1024

# define ABS(x)		((x < 0) ? -x : x)

typedef	struct				s_alloc_header
{
	int32_t					size;
	uint8_t					available;
	uint8_t					last;
}							t_alloc_header;

typedef	struct				s_zone_header
{
	struct s_zone			*next_zone;
}							t_zone_header;

typedef	struct				s_zone
{
	t_zone_header			header;
	t_alloc_header			first_alloc_header;
	//uint8_t				first allocation memory of 'size' octets of memory;
}							t_zone;

typedef	struct				s_malloc
{
	t_zone					*tiny_zone;
	size_t					tiny_zone_size;
	t_zone					*small_zone;
	size_t					small_zone_size;
}							t_malloc;

# include "prototypes_malloc.h"
# include "ft_malloc_tab.h"

#endif
