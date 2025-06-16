/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipexfirst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:44:36 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/16 12:46:06 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_f(t_command *cmd, t_shell *sh)
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
	end_f(cmd, sh);
}
