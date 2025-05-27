/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:28 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 10:12:56 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n(const char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] == '\0')
		return (ERROR);
	i = 1;
	while (s[i] == 'n')
		i++;
	if (s[i] == '\0')
		return (SUCCESS);
	return (ERROR);
}

int	ft_echo(char **argv)
{
	int	i;
	int	new;

	i = 1;
	new = 1;
	while (argv[i] && is_n(argv[i]) == SUCCESS)
	{
		new = 0;
		i++;
	}
	while (argv[i])
	{
		if (i > 1 && (new || i > 2 || is_n(argv[1]) != SUCCESS))
			ft_printf(" ");
		ft_printf("%s", argv[i]);
		i++;
	}
	if (new)
		ft_printf("\n");
	return (SUCCESS);
}
