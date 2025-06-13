/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:45:03 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 15:02:17 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

static t_token	*detach_split(t_token **cur)
{
	t_token	*start;
	t_token	*prev;

	start = *cur;
	prev = NULL;
	while (*cur && (*cur)->type != T_PIPE)
	{
		prev = *cur;
		*cur = (*cur)->next;
	}
	if (*cur && (*cur)->type == T_PIPE)
	{
		prev = *cur;
		*cur = (*cur)->next;
		prev->next = NULL;
	}
	return (start);
}

t_token	**split_commands(t_token *tokens)
{
	t_token	**splits;
	t_token	*cur;
	int		cmd_count;
	int		i;

	if (!tokens)
		return (NULL);
	cmd_count = count_cmds(tokens);
	splits = malloc(sizeof(t_token *) * (cmd_count + 1));
	if (!splits)
		return (NULL);
	cur = tokens;
	i = 0;
	while (i < cmd_count)
	{
		splits[i] = detach_split(&cur);
		i++;
	}
	splits[i] = NULL;
	return (splits);
}

void	free_cmd_splits(t_token **cmd_splits)
{
	int	i;

	if (!cmd_splits)
		return ;
	i = 0;
	while (cmd_splits[i])
	{
		free_tokens(cmd_splits[i]);
		i++;
	}
	free(cmd_splits);
}
