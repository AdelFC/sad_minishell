/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:36 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/02 11:15:05 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_single_external(t_command *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid < 0)
	{
		ft_printf_error("minishell: fork failed\n");
		sh->last_status = 1;
		return (1);
	}
	else if (pid == 0)
	{
		b_sig();
		if (apply_redirections(cmd->redirs) == ERROR)
			exit(EXIT_FAILURE);
		path = find_path(cmd->argv[0], sh->envp);
		if (!path)
			handle_command_error(sh, cmd, 127);
		execve(path, cmd->argv, sh->envp);
		free(path);
		handle_command_error(sh, cmd, 126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
	return (sh->last_status);
}

int	exec_single_builtin_cmd(t_command *cmd, t_shell *sh)
{
	int	in_save;
	int	out_save;
	int	status;

	if (handle_redirections_and_save(cmd, sh, &in_save, &out_save))
		return (sh->last_status);
	if (handle_exit_builtin(cmd, sh, in_save, out_save))
		return (1);
	status = exec_builtin(cmd->argv, sh);
	restore_std_fds(in_save, out_save);
	return (status);
}

int	exec_commands(t_shell *sh)
{
	t_command	*cmd;

	cmd = sh->cmds;
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		if (cmd && cmd->redirs)
			handle_redir_without_cmd(cmd);
		return (sh->last_status);
	}
	if (!cmd->next)
	{
		if (is_builtin(cmd->argv[0]) == ERROR)
			sh->last_status = exec_single_builtin_cmd(cmd, sh);
		else
			sh->last_status = exec_single_external(cmd, sh);
		cleanup_heredocs_list(cmd);
	}
	else
	{
		ft_pipe(sh, &sh->last_status);
		cleanup_heredocs_list(cmd);
	}
	return (sh->last_status);
}
