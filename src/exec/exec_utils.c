/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:43:09 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 12:12:20 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	cleanup_heredocs_list(t_command *cmd)
{
	while (cmd)
	{
		close_heredoc_fds(cmd->redirs);
		cmd = cmd->next;
	}
}

void	wait_for_single(pid_t pid, t_shell *sh)
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

void	restore_std_fds(int in_save, int out_save)
{
	dup2(in_save, STDIN_FILENO);
	dup2(out_save, STDOUT_FILENO);
	close(in_save);
	close(out_save);
}
