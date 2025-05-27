/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:10:11 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 18:37:33 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_first(t_command *cmd, t_shell *sh)
{
	char	*path;
	int		status;

	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (dup2(cmd->fd[1], STDOUT_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	if (is_builtin(cmd->argv[0]) == ERROR)
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit(status);
	}
	path = find_path(cmd->argv[0], sh->envp);
	if (!path)
		handle_command_error(sh, cmd, 127);
	execve(path, cmd->argv, sh->envp);
	free(path);
	handle_command_error(sh, cmd, 126);
}

static void	process_middle(int prev_fd, t_command *cmd, t_shell *sh)
{
	char	*path;
	int		status;

	if (dup2(prev_fd, STDIN_FILENO) == -1
		|| dup2(cmd->fd[1], STDOUT_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	close(prev_fd);
	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->argv[0]) == ERROR)
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit(status);
	}
	path = find_path(cmd->argv[0], sh->envp);
	if (!path)
		handle_command_error(sh, cmd, 127);
	execve(path, cmd->argv, sh->envp);
	free(path);
	handle_command_error(sh, cmd, 126);
}

static void	process_last(int prev_fd, t_command *cmd, t_shell *sh)
{
	char	*path;
	int		status;

	if (dup2(prev_fd, STDIN_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(prev_fd);
	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->argv[0]) == ERROR)
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit(status);
	}
	path = find_path(cmd->argv[0], sh->envp);
	if (!path)
		handle_command_error(sh, cmd, 127);
	execve(path, cmd->argv, sh->envp);
	free(path);
	handle_command_error(sh, cmd, 126);
}

void	ft_pipe(t_shell *sh, int *last_status)
{
	int			prev_fd;
	pid_t		pid;
	t_command	*cur;

	prev_fd = -1;
	cur = sh->cmds;
	while (cur)
	{
		if (cur->next && pipe(cur->fd) < 0)
			ft_printf_error(ERR_PIPE);
		pid = fork();
		if (pid < 0)
			ft_printf_error(ERR_FORK);
		else if (pid == 0)
		{
			if (!cur->next)
				process_last(prev_fd, cur, sh);
			else if (prev_fd == -1)
				process_first(cur, sh);
			else
				process_middle(prev_fd, cur, sh);
		}
		if (cur->next)
		{
			close(cur->fd[1]);
			if (prev_fd != -1)
				close(prev_fd);
			prev_fd = cur->fd[0];
		}
		cur = cur->next;
	}
	while (wait(last_status) > 0)
		continue ;
}
