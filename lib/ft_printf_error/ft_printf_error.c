/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:43:13 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:52:12 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_error.h"

int	ft_pprintf_error_args(char c, va_list args, int *value)
{
	if (c == 'c')
		ft_pputchar((char)va_arg(args, int), value);
	else if (c == 's')
		ft_pputstr((char *)va_arg(args, char *), value);
	else if (c == 'p')
		ft_pprintf_error_p(va_arg(args, unsigned long long), "0123456789abcdef",
			value, 'p');
	else if (c == 'd' || c == 'i')
		ft_pputnbr(va_arg(args, int), value);
	else if (c == 'u')
		ft_pputnbr_unsigned(va_arg(args, unsigned int), value);
	else if (c == 'x')
		ft_pputnbr_base(va_arg(args, unsigned int), "0123456789abcdef", value,
			'x');
	else if (c == 'X')
		ft_pputnbr_base(va_arg(args, unsigned int), "0123456789ABCDEF", value,
			'X');
	else if (c == '%')
		ft_pputchar('%', value);
	return (1);
}

int	ft_printf_error(const char *s, ...)
{
	size_t	i;
	va_list	args;
	int		value;

	value = 0;
	va_start(args, s);
	i = 0;
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1])
		{
			ft_pprintf_error_args(s[i + 1], args, &value);
			i++;
		}
		else
			ft_pputchar(s[i], &value);
		i++;
	}
	va_end(args);
	return (value);
}
