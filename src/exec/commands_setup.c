/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:27:29 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 10:01:00 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirs(t_redir *redir)
{
	t_redir	*cur;
	t_redir	*next;

	cur = redir;
	while (cur)
	{
		next = cur->next;
		if (cur->type == T_HEREDOC_DELIM && cur->heredoc_fd > 2)
		{
			close(cur->heredoc_fd);
			cur->heredoc_fd = -1;
		}
		if (cur->filename)
			free(cur->filename);
		free(cur);
		cur = next;
	}
}

void	free_commands(t_command *cmds)
{
	t_command	*cur;
	t_command	*next;
	int			i;

	cur = cmds;
	while (cur)
	{
		next = cur->next;
		if (cur->argv)
		{
			i = 0;
			while (cur->argv[i])
			{
				free(cur->argv[i]);
				i++;
			}
			free(cur->argv);
		}
		if (cur->redirs)
			free_redirs(cur->redirs);
		free(cur);
		cur = next;
	}
}
