/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_tab.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arobion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 16:28:41 by arobion           #+#    #+#             */
/*   Updated: 2020/10/23 16:45:31 by arobion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_TAB_H
# define FT_MALLOC_TAB_H
# define SIZE_USED_TAB 4096 * 5


typedef struct				s_header
{
	uint32_t				index;
	uint32_t				padding; // padding to align 
}							t_header;

typedef struct				s_zone_a
{
	struct s_zone			*next;
	uint32_t				available_size;
	uint32_t				padding; // padding to align 
}							t_zone_a;

typedef struct				s_free_space
{
	void					*address;
	struct s_free_space		*next;
	uint32_t				index;
	uint32_t				padding; // padding to align 
// 	uint32_t				size;
}							t_free_space;

typedef struct				s_mem_pool
{
	void					*address_start;
	size_t					size;
}							t_mem_pool;

typedef struct				s_used_space
{
	void					*address;
	struct s_free_spaces	*node;
}							t_used_space;

typedef struct				s_infos
{
	uint32_t				magic; // == 0x334C4152
	uint32_t				index_used_tab;
	t_free_space			*address_next_free_node;

	t_zone_a				*tiny;
	t_zone_a				*small;

	t_used_space			*address_used_space[128];
	t_used_space			**next_tabs_address;
	t_free_space			*tab_free_spaces[64];
	t_mem_pool				pool_tiny_free;

}							t_infos;


		// INIT
void				*create_master_zone(void);
void				*init_used_tab(void);
void				*init_free_space(void);


		// MALLOC
void				*get_allocation(t_infos *infos, size_t size);
t_free_space		*get_free_node(size_t size);
void				save_in_used(t_free_space *node);
void				fill_alloc_header(t_free_space *node);

		// FREE
t_used_space		*read_alloc_header(void *address);
void				save_in_free(t_used_space *entry);

		// ZONE UTIL
void				*allocate_new_zone(size_t size);

#endif
