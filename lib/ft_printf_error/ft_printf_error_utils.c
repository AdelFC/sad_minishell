/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:43:26 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/17 16:43:26 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_error.h"

void	ft_pputchar(char c, int *value)
{
	write(2, &c, 1);
	(*value)++;
}

void	ft_pputstr(char *str, int *value)
{
	if (!str)
		return (ft_pputstr("(null)", value));
	while (*str)
		ft_pputchar(*str++, value);
}

void	ft_pputnbr(int nb, int *value)
{
	if (nb == INT_MIN)
	{
		ft_pputstr("-2147483648", value);
		return ;
	}
	if (nb < 0)
	{
		ft_pputchar('-', value);
		nb = -nb;
	}
	if (nb >= 10)
		ft_pputnbr(nb / 10, value);
	ft_pputchar((nb % 10) + '0', value);
}

void	ft_pputnbr_unsigned(unsigned int nb, int *value)
{
	if (nb >= 10)
		ft_pputnbr_unsigned(nb / 10, value);
	ft_pputchar((nb % 10) + '0', value);
}
