/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:28 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/20 10:36:52 by afodil-c         ###   ########.fr       */
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

int    ft_echo(char **argv)
{
    int    i;
    int    new;
    int    first;

    i = 1;
    new = 1;
    while (argv[i] && is_n(argv[i]) == SUCCESS)
    {
        new = 0;
        i++;
    }
    first = 1;
    while (argv[i])
    {
        if (!first)
            ft_printf(" ");
        ft_printf("%s", argv[i]);
        first = 0;
        i++;
    }
    if (new)
        ft_printf("\n");
    return (SUCCESS);
}
