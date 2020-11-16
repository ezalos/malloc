/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_realloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:20:23 by ezalos            #+#    #+#             */
/*   Updated: 2020/11/16 20:20:24 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"


int main(void)
{
	void *ptr;
	void *ptr2;

	ptr = our_malloc(151);
	printf("After first allocation\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");
	// sleep(5);

	ptr = our_realloc(ptr, 233);
	printf("After bigger reallocation with space\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");
	// sleep(5);

	ptr = our_realloc(ptr, 151);
	printf("After smaller reallocation\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");
	// sleep(5);

	ptr2 = our_malloc(125);
	printf("After new malloc allocation\n");
	printf("ptr2 = %p\n", ptr2);
	show_alloc_mem();
	printf("\n\n");
	// sleep(5);

	ptr = our_realloc(ptr, 273);
	printf("After bigger reallocation without space\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");

	our_free(ptr);
	our_free(ptr2);

	ptr = our_malloc(100);
	printf("After tiny allocation\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");

	ptr = our_realloc(ptr, 1000);
	printf("After reallocation with small size\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");

	ptr = our_realloc(ptr, 100);
	printf("After reallocation with tiny size\n");
	printf("ptr = %p\n", ptr);
	show_alloc_mem();
	printf("\n\n");

	return (0);
}
