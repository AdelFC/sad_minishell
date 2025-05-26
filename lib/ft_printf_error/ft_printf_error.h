/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:42:55 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:51:43 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_ERROR_H
# define FT_PRINTF_ERROR_H
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <unistd.h>

int		ft_printf_error(const char *s, ...);
int		ft_pprintf_error_args(char c, va_list args, int *value);
void	ft_pputchar(char c, int *value);
void	ft_pputstr(char *str, int *value);
void	ft_pputnbr(int nb, int *value);
void	ft_pputnbr_unsigned(unsigned int nb, int *value);
void	ft_pputnbr_base(unsigned long long nbr, char *base, int *value,
			char type);
void	ft_pprintf_error_p(unsigned long long nbr, char *base, int *value,
			char type);
#endif
