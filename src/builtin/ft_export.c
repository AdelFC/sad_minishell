/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:13 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/04 11:27:03 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_without_value(const char *arg, t_env **env)
{
	char	*name;
	int		ret;

	name = get_var_name(arg);
	if (!name)
		return (1);
	if (!env_get(*env, name))
	{
		ret = env_set(env, name, "");
		if (ret == ERROR)
		{
			free(name);
			return (1);
		}
	}
	free(name);
	return (SUCCESS);
}

static void	print_env_vars(t_env *env)
{
	env = sort_env_alpha(env);
	while (env)
	{
		if (env->value)
			printf("export %s=\"%s\"\n", env->name, env->value);
		else
			printf("export %s\n", env->name);
		env = env->next;
	}
}

static int	export_arg(const char *arg, t_env **env)
{
	char	*name;
	int		ret;

	name = get_var_name(arg);
	if (!name)
		return (1);
	if (is_valid_identifier(name) == ERROR)
		ret = 1;
	else if (ft_strchr(arg, '='))
		ret = export_with_value(arg, env);
	else
		ret = export_without_value(arg, env);
	if (ret == 1)
		ft_printf_error("export: %s: not a valid identifier\n", arg);
	free(name);
	return (ret);
}

static int	process_export_args(char **argv, t_env **env)
{
	int	i;
	int	status;
	int	ret;

	i = 1;
	status = SUCCESS;
	while (argv[i])
	{
		ret = export_arg(argv[i], env);
		if (ret == 1)
			status = 1;
		i++;
	}
	return (status);
}

int	ft_export(char **argv, t_env **env)
{
	if (!argv[1])
	{
		print_env_vars(*env);
		return (SUCCESS);
	}
	return (process_export_args(argv, env));
}
