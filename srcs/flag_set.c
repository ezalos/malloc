/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 14:28:39 by ldevelle          #+#    #+#             */
/*   Updated: 2020/11/09 17:23:44 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static uint8_t		flag_set(uint8_t flag, uint8_t category, uint8_t option)
{
	return (((~category) & flag) | (option & category));
}

uint8_t				flag_set_pos(uint8_t flag, uint8_t option)
{
	return (flag_set(flag, HDR_POS, option));
}

uint8_t				flag_set_type(uint8_t flag, uint8_t option)
{
	return (flag_set(flag, HDR_TYPE, option));
}

uint8_t				flag_set_availabilty(uint8_t flag, uint8_t option)
{
	return (flag_set(flag, HDR_AVAILABLE, option));
}
