/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:17 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/04 11:45:01 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_cd_errors(char **argv, t_env **env, char **path_out)
{
	if (!argv[1])
	{
		*path_out = env_get(*env, "HOME");
		if (!*path_out)
		{
			ft_printf_error(ERR_CD_HOME_NOT_SET);
			return (ERROR);
		}
	}
	else if (argv[2])
	{
		ft_printf_error(ERR_CD_TOO_MANY_ARGS);
		return (ERROR);
	}
	else
	{
		*path_out = argv[1];
	}
	return (SUCCESS);
}

static int	update_env_from_cwd(t_env **env,
				const char *var_name, int print_err)
{
	char	*cwd;
	int		ret;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (print_err)
			ft_printf_error(ERR_CD_CWD);
		return (ERROR);
	}
	ret = env_set(env, var_name, cwd);
	free(cwd);
	if (ret == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	ft_cd(char **argv, t_env **env)
{
	char	*target;

	if (handle_cd_errors(argv, env, &target) == ERROR)
		return (1);
	if (chdir(target) != 0)
		return (ft_printf_error(ERR_CD_NO_SUCH_DIR, target), 1);
	if (update_env_from_cwd(env, "OLDPWD", 1) == ERROR)
		return (1);
	if (update_env_from_cwd(env, "PWD", 0) == ERROR)
		return (1);
	return (SUCCESS);
}
