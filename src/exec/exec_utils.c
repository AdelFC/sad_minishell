/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:43:09 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 01:10:41 by afodil-c         ###   ########.fr       */
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
