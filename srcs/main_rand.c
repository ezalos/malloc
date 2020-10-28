/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_rand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 15:01:26 by arobion           #+#    #+#             */
/*   Updated: 2020/10/27 15:44:04 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SIZE_TAB 10
#define NB_TEST 10000000
#define SIZE_ALLOC 16

int			get_size_alloc()
{
	return (rand() % (2 << 16));
}

void		test_write2(void *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		((char*)mem)[i++] = 0x42;
}

int8_t		unit_test(char **tab)
{
	size_t		size;
	int			r;

	r = rand() % SIZE_TAB;

	printf("r = %d\n", r);
	our_free(tab[r]);
	printf("AFTER FREE\n");
	size = SIZE_ALLOC;
	if (!(tab[r] = our_malloc(size)))
		return (ERROR);
	printf("AFTER NEW MALLOC\n");
	test_write(tab[r], secure_align_size(size));
	return (SUCCESS);
}

char		**init(void)
{
	char		**tab;
	int			i;

	write(1, "v", 1);
	if (!(tab = our_malloc(sizeof(void *) * SIZE_TAB)))
		return (NULL);
	write(1, "^", 1);
	i = 0;
	write(1, "\n", 1);
	while (i < SIZE_TAB)
	{
		write(1, "x", 1);
		if (!(tab[i] = our_malloc(SIZE_ALLOC)))
			return (NULL);
		i++;
	}
	return (tab);
}

void		finish(char **tab)
{
	int			i;
	i = 0;
	while (i < SIZE_TAB)
	{
		write(1, "o", 1);
		our_free(tab[i]);
		i++;
	}
	our_free(tab);

}

int			main(int ac, char **av)
{
	char		**tab;
	int			i;

	(void)ac;
	(void)av;
	srand(42);

	write(1, "Begin tests\n", 12);
	printf("SIZE TAB = %d\n", SIZE_TAB);
	if (!(tab = init()))
		return (10);
	write(1, "\n", 1);
	i = -1;
	// while (++i < NB_TEST)
	while (++i < 5)
	{
		write(1, "?", 1);
		if (ERROR == unit_test(tab))
			return (10);
	}
	write(1, "\n", 1);
	finish(tab);

	write(1, "\nEnd tests\n", 11);
	return (0);
}