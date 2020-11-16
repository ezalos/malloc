/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_defrag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:20:31 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:20:32 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int		main(int ac, char **av)
{
	void	*ptr1;
	void	*ptr2;
	void	*ptr3;
	void	*ptr4;

	(void)ac;
	(void)av;
	printf("------ Defragmentation on with right side -------\n\n");
	ptr1 = our_malloc(60);
	ptr2 = our_malloc(60);
	ptr3 = our_malloc(60);
	ptr4 = our_malloc(60);
	printf("After 4 allocations\n");
	show_alloc_mem();

	our_free(ptr4);
	printf("After free of the last pointer\n");
	show_alloc_mem();
	printf("\n\n");

	printf("------ Defragmentation on with left side -------\n\n");
	ptr4 = our_malloc(60);
	printf("After 4 allocations\n");
	show_alloc_mem();
	printf("\n");

	our_free(ptr2);
	printf("After free of the second pointer\n");
	show_alloc_mem();
	printf("\n");


	our_free(ptr3);
	printf("After free of the third pointer\n");
	show_alloc_mem();
	printf("\n\n");


	printf("-------- Defragmentation on both sides ---------\n\n");
	ptr2 = our_malloc(60);
	ptr3 = our_malloc(60);
	printf("After 4 allocations");
	show_alloc_mem();
	printf("\n");


	our_free(ptr2);
	our_free(ptr4);
	printf("After free of the second and the fourth pointers\n");
	show_alloc_mem();
	printf("\n");

	our_free(ptr3);
	printf("\nAfter free of the third pointer\n");
	show_alloc_mem();



	return (0);
}
