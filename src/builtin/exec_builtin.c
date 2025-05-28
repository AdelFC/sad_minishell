/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:02:53 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 21:48:53 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (SUCCESS);
	if ((!ft_strncmp(cmd, "cd", 3) && cmd[2] == '\0') || (!ft_strncmp(cmd,
				"pwd", 4) && cmd[3] == '\0') || (!ft_strncmp(cmd, "env", 4)
			&& cmd[3] == '\0') || (!ft_strncmp(cmd, "export", 7)
			&& cmd[6] == '\0') || (!ft_strncmp(cmd, "unset", 6)
			&& cmd[5] == '\0') || (!ft_strncmp(cmd, "exit", 5)
			&& cmd[4] == '\0') || (!ft_strncmp(cmd, "echo", 5)
			&& cmd[4] == '\0'))
		return (ERROR);
	return (SUCCESS);
}

int	exec_builtin(char **argv, t_shell *sh)
{
	if (!argv || !argv[0])
		return (ERROR);
	if (!ft_strncmp(argv[0], "cd", 3) && argv[0][2] == '\0')
		return (ft_cd(argv, &sh->env));
	if (!ft_strncmp(argv[0], "pwd", 4) && argv[0][3] == '\0')
		return (ft_pwd(argv));
	if (!ft_strncmp(argv[0], "env", 4) && argv[0][3] == '\0')
		return (ft_env(argv, &sh->env));
	if (!ft_strncmp(argv[0], "export", 7) && argv[0][6] == '\0')
		return (ft_export(argv, &sh->env));
	if (!ft_strncmp(argv[0], "unset", 6) && argv[0][5] == '\0')
		return (ft_unset(argv, &sh->env));
	if (!ft_strncmp(argv[0], "echo", 5) && argv[0][4] == '\0')
		return (ft_echo(argv));
	return (ERROR);
}
