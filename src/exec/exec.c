/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:36 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 11:31:56 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_single_external(t_command *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid < 0)
	{
		ft_printf_error("minishell: fork failed\n");
		return (sh->last_status);
	}
	else if (pid == 0)
	{
		if (apply_redirections(cmd->redirs) == ERROR)
			exit(EXIT_FAILURE);
		path = find_path(cmd->argv[0], sh->envp);
		if (!path)
			handle_command_error(cmd, 127);
		execve(path, cmd->argv, sh->envp);
		free(path);
		handle_command_error(cmd, 126);
	}
	if (waitpid(pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			sh->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_status = 128 + WTERMSIG(status);
	}
	return (sh->last_status);
}

int	exec_commands(t_shell *sh)
{
	t_command	*cmd;

	cmd = sh->cmds;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (sh->last_status);
	if (!cmd->next && is_builtin(cmd->argv[0]) == ERROR)
	{
		if (cmd->redirs && apply_redirections(cmd->redirs) == ERROR)
			return (sh->last_status);
		sh->last_status = exec_builtin(cmd->argv, sh);
	}
	else if (!cmd->next)
	{
		sh->last_status = exec_single_external(cmd, sh);
	}
	else
		ft_pipe(sh, &sh->last_status);
	return (sh->last_status);
}
