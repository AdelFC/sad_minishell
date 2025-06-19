/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:36 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/19 13:44:30 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_exec(t_command *cmd, t_shell *sh)
{
	char	*path;
	char	**envp;

	signal(SIGPIPE, sigpipe_handler);
	b_sig();
	if (apply_redirections(cmd->redirs) == ERROR)
	{
		free_shell(sh);
		exit(EXIT_FAILURE);
	}
	path = find_path(cmd->argv[0], sh->env);
	if (!path)
		handle_command_error(sh, cmd, 127);
	envp = env_list_to_array(sh->env);
	execve(path, cmd->argv, envp);
	free_array(envp);
	free(path);
	handle_command_error(sh, cmd, 126);
}

static void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	setup_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
}

static void	restore_signals_parent(void)
{
	signal(SIGINT, handle_sigint);
}

static void	update_status_from_wait(int status, t_shell *sh)
{
	if (WIFEXITED(status))
		sh->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_status = 128 + WTERMSIG(status);
}

int	exec_single_external(t_command *cmd, t_shell *sh)
{
	pid_t	pid;
	int		status = 0;

	pid = fork();
	if (pid < 0)
	{
		ft_printf_error("minishell: fork failed\n");
		sh->last_status = 1;
		return (1);
	}
	else if (pid == 0)
	{
		setup_signals_child();
		child_process_exec(cmd, sh);
	}
	else
	{
		setup_signals_parent();
		waitpid(pid, &status, 0);
		restore_signals_parent();
		update_status_from_wait(status, sh);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			ft_printf("\n");
	}
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
	if (!cmd || !cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == 0)
	{
		if (cmd && cmd->redirs)
			handle_redir_without_cmd(cmd);
		sh->last_status = 0;
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
