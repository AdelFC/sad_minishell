/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:27:29 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 17:27:06 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Properly frees the memory allocated for the t_command structure
*/

static void	free_redirs(t_redir *redir)
{
	t_redir	*cur;
	t_redir	*next;

	cur = redir;
	while (cur)
	{
		next = cur->next;
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
