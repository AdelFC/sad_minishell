/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:16 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 11:28:50 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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

static int	count_exit_args(char **argv)
{
	int	i;
	int	count;

	i = 1;
	count = 0;
	while (argv[i])
	{
		count++;
		i++;
	}
	return (count);
}

static long long	ft_atoll(const char *str)
{
	int			sign;
	long long	result;
	int			i;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_exit(char **argv, t_shell *sh)
{
	long long	code;
	int			argc;

	code = 0;
	argc = count_exit_args(argv);
	if (argc > 1)
		return (ft_printf_error(ERR_EXIT_TOO_MANY_ARGS), 1);
	if (argc == 1)
	{
		if (is_numeric(argv[1]) == ERROR)
		{
			ft_printf_error("minishell: exit: %s: numeric argument required\n",
				argv[1]);
			free_shell(sh);
			exit(2);
		}
		code = ft_atoll(argv[1]);
	}
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && getppid() != 1)
		printf("exit\n");
	free_shell(sh);
	exit((unsigned char)code);
}
