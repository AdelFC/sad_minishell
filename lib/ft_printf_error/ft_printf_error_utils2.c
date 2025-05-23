/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:43:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/17 16:44:09 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_error.h"
#include <stdio.h>

void	ft_putnbr_base(unsigned long long nbr, char *base, int *value,
		char type)
{
	if (nbr >= 16)
		ft_putnbr_base(nbr / 16, base, value, type);
	ft_putchar(base[nbr % 16], value);
}

void	ft_printf_error_p(unsigned long long nbr, char *base, int *value,
		char type)
{
	if (nbr == 0)
	{
		ft_putstr("(nil)", value);
		return ;
	}
	ft_putstr("0x", value);
	ft_putnbr_base(nbr, base, value, type);
}
