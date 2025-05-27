/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:16 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 11:36:26 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (ERROR);
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (s[i] == '\0')
		return (ERROR);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	ft_exit(char **argv, t_shell *sh)
{
	long	code;
	int		argc;
	int		i;

	code = 0;
	argc = 0;
	i = 1;
	while (argv[i])
	{
		argc++;
		i++;
	}
	if (argc > 1)
		return (ft_printf_error(ERR_EXIT_TOO_MANY_ARGS), ERROR);
	if (argc == 1)
	{
		if (is_numeric(argv[1]) == ERROR)
		{
			ft_printf_error(ERR_EXIT_NUM_ARG, argv[1]);
			free_shell(sh);
			sh = NULL;
			exit(2);
		}
		code = ft_atoi(argv[1]);
	}
	ft_printf("exit\n");
	free_shell(sh);
	exit((int)code);
}
