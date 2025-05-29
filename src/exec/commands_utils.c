/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 00:34:48 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 15:11:09 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	argv_len(char **argv)
{
	int	len;

	len = 0;
	if (!argv)
		return (0);
	while (argv[len])
		len++;
	return (len);
}

int	add_argv(t_command *cmd, const char *arg)
{
	char	**new;
	int		old;
	int		i;

	old = argv_len(cmd->argv);
	new = malloc(sizeof(char *) * (old + 2));
	if (!new)
		return (ERROR);
	i = 0;
	while (i < old)
	{
		new[i] = cmd->argv[i];
		i++;
	}
	new[i] = ft_strdup(arg);
	if (!new[i])
		return (free(new), ERROR);
	new[i + 1] = NULL;
	if (cmd->argv)
		free(cmd->argv);
	cmd->argv = new;
	return (SUCCESS);
}

int	add_redir(t_command *cmd, int type, const char *filename)
{
	t_redir	*red;
	t_redir	*cur;

	red = malloc(sizeof(*red));
	if (!red)
		return (ERROR);
	red->type = type;
	red->heredoc_fd = -1;
	if (!filename)
		return (free(red), ERROR);
	red->filename = ft_strdup(filename);
	if (!red->filename)
		return (free(red), ERROR);
	red->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = red;
	else
	{
		cur = cmd->redirs;
		while (cur->next)
			cur = cur->next;
		cur->next = red;
	}
	return (SUCCESS);
}
