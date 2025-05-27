/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:43:09 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 00:44:03 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_heredocs_list(t_command *cmd)
{
	while (cmd)
	{
		close_heredoc_fds(cmd->redirs);
		cmd = cmd->next;
	}
}
