/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_rand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:20:48 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:20:49 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "head.h"

#define SIZE_TAB	5000
#define NB_TEST		1000000
#define SIZE_ALLOC	16
#define TINY		(RES_TINY * TINY_SIZE_MAX_FACTOR)
#define SMALL		(RES_SMALL * SMALL_SIZE_MAX_FACTOR)
#define THREAD_NB		1
#define TEST_MALLOC		1
#define TEST_REALLOC	2
#define TEST_FREE		3

typedef struct s_thread_infos
{
	int			nb_tests;
	pthread_t	*thread_tab;
}				t_thread_infos;

// typedef struct				s_alloc_test
// {
// 	pthread_t				thread_nb;
// 	void					*mem;
// 	void					*old_mem;
// 	size_t					size;
// 	size_t					old_size;
// 	uint8_t					test_type;
// 	uint8_t					old_test_type;
// 	uint8_t					retval;
//
// }							t_alloc_test;

pthread_mutex_t g_lock;

size_t		size_tab[THREAD_NB][SIZE_TAB];

// void		*our_malloc(size_t size)
// {
// 	void	*mem;
//
// 	mem = malloc(size);
// 	return (mem);
// }
//
// void		*our_realloc(void *ptr, size_t size)
// {
// 	void	*mem;
//
// 	mem = realloc(ptr, size);
// 	return (mem);
// }
//
// void		our_free(void *ptr)
// {
// 	free(ptr);
// }

size_t			get_align_size(size_t size)
{
	// return (size + (8 - (size % 8)));
	return secure_align_size(size);
}


void		test_write(void *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		((char*)mem)[i++] = (uint8_t)size;
}

void		test_read(void *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		if (((uint8_t*)mem)[i++] != (uint8_t)size)
		{
			write(1, "ERROR USER MEMORY HAS BEEN ALTERED\n", 35);
			break ;
		}
}

size_t			get_size_alloc()
{
	size_t	size_type;
	size_t	size;

	size_type = 3;
	while (size_type == 3)
		size_type = rand() % 3;
	if (size_type == 0)
		size = (size_t)rand() % TINY;
	else if (size_type == 1)
		size = TINY + ((size_t)rand() % (SMALL - TINY));
	else
		size = SMALL + ((size_t)rand() % (2 << 18));
	return (size);
}

void		print_unit_test(t_alloc_test *tab)
{
	pthread_mutex_lock(&g_lock);
	printf("%8lu ", (unsigned long)pthread_self());
	if (tab->test_type == TEST_MALLOC)
		printf("malloc:  ");
	else if (tab->test_type == TEST_REALLOC)
		printf("realloc: ");
	else
		printf("free:    ");
	printf(" %6lu", tab->size);
	printf(" (%6lu)", tab->old_size);
	printf(" %p", tab->mem);
	printf(" (%p)", tab->old_mem);
	printf(" %d\n", tab->retval);
	pthread_mutex_unlock(&g_lock);
}

int8_t		unit_test_malloc(t_alloc_test *tab)
{
	tab->old_size = tab->size;
	tab->old_mem = tab->mem;
	our_free(tab->mem);
	tab->size = get_size_alloc();
	if (!(tab->mem = our_malloc(tab->size)))
		return (ERROR);
	return (SUCCESS);
}

int8_t		unit_test_realloc(t_alloc_test *tab)
{
	tab->old_size = tab->size;
	tab->old_mem = tab->mem;
	tab->size = get_size_alloc();
	if (!(tab->mem = our_realloc(tab->mem, tab->size)))
		return (ERROR);
	return (SUCCESS);
}

int8_t		unit_test(t_alloc_test *tab)
{
	size_t	type;
	int8_t	retval;
	int			r;

	r = rand() % SIZE_TAB;
	type = rand() % 100;
	test_read(tab[r].mem, get_align_size(tab[r].size));
	if (type < 35)
	{
		tab[r].test_type = TEST_REALLOC;
		retval = unit_test_realloc(&tab[r]);
	}
	else
	{
		tab[r].test_type = TEST_MALLOC;
		retval = unit_test_malloc(&tab[r]);
	}
	if ((size_t)tab->mem % 8 != 0)
		write(1, "-------------NOT ALIGNED PTR-------------\n", 42);
	type = rand() % 100;
	if (type > 100)
	{
		tab[r].test_type = TEST_FREE;
		our_free((void*)(((size_t)rand() << 32) + (size_t)rand()));
	}
	tab[r].retval = retval;
	test_write(tab[r].mem, get_align_size(tab[r].size));
	print_unit_test(&tab[r]);
	return (retval);
}

size_t				get_thread_index(pthread_t current_thread, pthread_t *thread_tab)
{
	size_t	index;

	index = 0;
	while (index < THREAD_NB)
	{
		if (thread_tab[index] == current_thread)
			return index;
		index++;
	}
	return (0);
}

t_alloc_test		*init(void)
{
	t_alloc_test		*tab;
	int					i;

	if (!(tab = our_malloc(sizeof(t_alloc_test) * SIZE_TAB)))
		return (NULL);
	i = 0;
	while (i < SIZE_TAB)
	{
		tab[i].size = SIZE_ALLOC;
		if (!(tab[i].mem = our_malloc(tab[i].size)))
			return (NULL);
		test_write(tab[i].mem, get_align_size(tab[i].size));
		i++;
	}
	return (tab);
}

void		finish(t_alloc_test *tab)
{
	int			i;
	i = 0;
	while (i < SIZE_TAB)
	{
		our_free(tab[i].mem);
		i++;
	}
	our_free(tab);
}

void			*test_routine(void *thread_infos)
{
	t_alloc_test	*tab;
	int		i;

	i = -1;
	if (!(tab = init()))
		return (NULL);
	pthread_mutex_lock(&g_lock);
	printf("\nNB TEST = %d\n", ((t_thread_infos*)thread_infos)->nb_tests);
	pthread_mutex_unlock(&g_lock);
	while (++i < ((t_thread_infos*)thread_infos)->nb_tests)
	{
		if (ERROR == unit_test(tab))
			exit(1);
	}
	finish(tab);
	return (NULL);
}




int			main(int ac, char **av)
{
	t_thread_infos	thread_infos;
	int				i;
	pthread_t 		thread_tab[THREAD_NB];

	our_free((void*)(size_t)0xBADA55);
	if (pthread_mutex_init(&g_lock, NULL) != 0)
		printf("\n mutex init failed\n");
	if (ac > 1)
		thread_infos.nb_tests = atoi(av[1]);
	else
		thread_infos.nb_tests = NB_TEST;
	if (ac > 2)
		srand(atoi(av[2]));
	else
		srand(17);
	write(1, "\n", 1);
	i = -1;
	while (++i < THREAD_NB)
	{
		if (pthread_create(&thread_tab[i], NULL, &test_routine, (void*)&thread_infos))
			return (1);
	}
	i = -1;
	while (++i < THREAD_NB)
		if (pthread_join(thread_tab[i], NULL))
			return (1);
	pthread_mutex_destroy(&g_lock);
	return (0);
}
