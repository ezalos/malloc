/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 17:00:46 by ezalos            #+#    #+#             */
/*   Updated: 2020/10/23 15:23:05 by arobion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"
#include <stdio.h>

int		main()
{
	char *test;	
	int tot = 0;
	int i;
	while (tot < 1000000)
	{

		if(!(test = our_malloc(12)))
			return (0);
		i = 0;
		while (i < 10)
			test[i++]= 'X';
		test[i] = '\n';
		test[i + 1] = '\0';
// 		dprintf(1, "%s", test);
		tot++;
	}
}
