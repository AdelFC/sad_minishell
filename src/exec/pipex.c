/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:10:11 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 12:33:32 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cavaouquoi(t_command *cmd, t_shell *sh)
{
	char	*path;
	char	**envp;

	path = find_path(cmd->argv[0], sh->env);
	envp = NULL;
	if (!path)
	{
		ft_printf_error(ERR_MINISHELL_CMD_NOT_FOUND, cmd->argv[0]);
		exit(127);
	}
	envp = env_list_to_array(sh->env);
	execve(path, cmd->argv, envp);
	free_array(envp);
	ft_printf_error(ERR_MINISHELL_PERMISSION, cmd->argv[0]);
	free(path);
	exit(126);
}

void	process_first(t_command *cmd, t_shell *sh)
{
	int	status;

	if (dup2(cmd->fd[1], STDOUT_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	signal(SIGPIPE, sigpipe_handler);
	b_sig();
	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (!cmd->argv[0] || cmd->argv[0][0] == 0)
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit((unsigned char)status);
	}
	cavaouquoi(cmd, sh);
}

void	setup_pipes(t_command *cmd, int prev_fd)
{
	if (dup2(prev_fd, STDIN_FILENO) == -1
		|| dup2(cmd->fd[1], STDOUT_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(cmd->fd[0]);
	close(cmd->fd[1]);
	close(prev_fd);
}

void	process_middle(int prev_fd, t_command *cmd, t_shell *sh)
{
	int	status;

	setup_pipes(cmd, prev_fd);
	signal(SIGPIPE, sigpipe_handler);
	b_sig();
	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (!cmd->argv[0] || cmd->argv[0][0] == 0)
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit((unsigned char)status);
	}
	cavaouquoi(cmd, sh);
}

void	process_last(int prev_fd, t_command *cmd, t_shell *sh)
{
	int	status;

	if (dup2(prev_fd, STDIN_FILENO) == -1)
	{
		ft_printf_error(ERR_DUP2, strerror(errno));
		exit(EXIT_FAILURE);
	}
	close(prev_fd);
	signal(SIGPIPE, sigpipe_handler);
	b_sig();
	if (apply_redirections(cmd->redirs) == ERROR)
		exit(EXIT_FAILURE);
	if (!cmd->argv[0] || cmd->argv[0][0] == 0)
		exit(0);
	if (is_builtin(cmd->argv[0]))
	{
		status = exec_builtin(cmd->argv, sh);
		free_shell(sh);
		exit((unsigned char)status);
	}
	cavaouquoi(cmd, sh);
}

void	ft_pipe(t_shell *sh, int *last_status)
{
	int			prev_fd;
	int			last_fd;
	t_command	*cur;
	int			status;

	prev_fd = -1;
	cur = sh->cmds;
	while (cur)
	{
		handle_pipe_iteration(cur, sh, prev_fd);
		update_prev_fd(&prev_fd, cur);
		cur = cur->next;
	}
	last_fd = prev_fd;
	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
		{
			*last_status = 0;
			continue ;
		}
		*last_status = status;
	}
	if (last_fd != -1)
		close(last_fd);
	if (WIFEXITED(*last_status))
		*last_status = WEXITSTATUS(*last_status);
	else if (WIFSIGNALED(*last_status))
		*last_status = 128 + WTERMSIG(*last_status);
}
