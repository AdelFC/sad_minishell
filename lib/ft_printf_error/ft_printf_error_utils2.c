/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:43:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:53:25 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_error.h"
#include <stdio.h>

void	ft_pputnbr_base(unsigned long long nbr, char *base, int *value,
		char type)
{
	if (nbr >= 16)
		ft_pputnbr_base(nbr / 16, base, value, type);
	ft_pputchar(base[nbr % 16], value);
}

void	ft_pprintf_error_p(unsigned long long nbr, char *base, int *value,
		char type)
{
	if (nbr == 0)
	{
		ft_pputstr("(nil)", value);
		return ;
	}
	ft_pputstr("0x", value);
	ft_pputnbr_base(nbr, base, value, type);
}
