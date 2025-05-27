/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:05 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 22:22:05 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	remove_env_var(t_env **env_list, const char *name)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env_list;
	prev = NULL;
	while (cur)
	{
		if (!ft_strncmp(cur->name, name, ft_strlen(name) + 1))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env_list = cur->next;
			free(cur->name);
			free(cur->value);
			free(cur);
			return (SUCCESS);
		}
		prev = cur;
		cur = cur->next;
	}
	return (ERROR);
}

static int	process_unset_arg(const char *arg, t_env **env)
{
	int	res;

	if (!arg[0] || !(ft_isalpha(arg[0]) || arg[0] == '_'))
	{
		ft_printf_error(ERR_EXPORT_VALID_ID, arg);
		return (ERROR);
	}
	res = remove_env_var(env, arg);
	if (res == ERROR)
	{
		ft_printf_error(ERR_UNSET_NOT_FOUND);
		return (ERROR);
	}
	return (SUCCESS);
}

int	ft_unset(char **argv, t_env **env)
{
	int	i;
	int	status;
	int	ret;

	if (!argv[1])
	{
		ft_printf_error(ERR_UNSET_NO_ARGS);
		return (ERROR);
	}
	status = SUCCESS;
	i = 1;
	while (argv[i])
	{
		ret = process_unset_arg(argv[i], env);
		if (ret == ERROR)
			status = ERROR;
		i++;
	}
	return (status);
}
