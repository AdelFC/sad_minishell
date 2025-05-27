/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:36 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 23:40:45 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	wait_for_single(pid_t pid, t_shell *sh)
{
	int	status;

	if (waitpid(pid, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			sh->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_status = 128 + WTERMSIG(status);
	}
}

static void	close_heredoc_fds(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == T_HEREDOC_DELIM && redir->heredoc_fd > 2)
		{
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}

int	exec_single_external(t_command *cmd, t_shell *sh)
{
	pid_t	pid;
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
			handle_command_error(sh, cmd, 127);
		execve(path, cmd->argv, sh->envp);
		free(path);
		handle_command_error(sh, cmd, 126);
	}
	wait_for_single(pid, sh);
	return (sh->last_status);
}

int	exec_commands(t_shell *sh)
{
	t_command	*cmd;
	int		stdin_save;
	int		stdout_save;

	cmd = sh->cmds;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (sh->last_status);
	if (!cmd->next && is_builtin(cmd->argv[0]) == ERROR)
	{
		stdin_save = dup(STDIN_FILENO);
		stdout_save = dup(STDOUT_FILENO);
		if (cmd->redirs && apply_redirections(cmd->redirs) == ERROR)
		{
			dup2(stdin_save, STDIN_FILENO);
			dup2(stdout_save, STDOUT_FILENO);
			close(stdin_save);
			close(stdout_save);
			close_heredoc_fds(cmd->redirs);
			return (sh->last_status);
		}
		sh->last_status = exec_builtin(cmd->argv, sh);
		dup2(stdin_save, STDIN_FILENO);
		dup2(stdout_save, STDOUT_FILENO);
		close(stdin_save);
		close(stdout_save);
		close_heredoc_fds(cmd->redirs);
	}
	else if (!cmd->next)
	{
		sh->last_status = exec_single_external(cmd, sh);
		close_heredoc_fds(cmd->redirs);
	}
	else
	{
		ft_pipe(sh, &sh->last_status);
		cmd = sh->cmds;
		while (cmd)
		{
			close_heredoc_fds(cmd->redirs);
			cmd = cmd->next;
		}
	}
	return (sh->last_status);
}
