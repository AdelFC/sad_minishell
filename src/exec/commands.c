/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:48:58 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 10:39:07 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_pipe(t_command **current)
{
	t_command	*next;

	next = new_command();
	if (!next)
		return (ERROR);
	(*current)->next = next;
	*current = next;
	return (SUCCESS);
}

static int	handle_simple_token(t_command **cur, t_token *tok)
{
	if (tok->type == T_CMD || tok->type == T_ARG)
		return (add_argv(*cur, tok->value));
	if (tok->type == T_PIPE)
		return (handle_pipe(cur));
	return (ERROR);
}

static int	handle_redirection(t_command *cur, t_token **token_i)
{
	t_token		*t;
	int			ret;

	t = *token_i;
	if (t->type == T_INFILE_OPERATOR)
		ret = add_redir(cur, T_INFILE_FILE, t->next->value);
	else if (t->type == T_OUTFILE_OPERATOR)
		ret = add_redir(cur, T_OUTFILE_FILE, t->next->value);
	else if (t->type == T_APPEND_OPERATOR)
		ret = add_redir(cur, T_APPEND_FILE, t->next->value);
	else if (t->type == T_HEREDOC_OPERATOR)
		ret = add_redir(cur, T_HEREDOC_DELIM, t->next->value);
	else
		return (ERROR);
	if (ret == ERROR)
		return (ERROR);
	*token_i = t->next;
	return (SUCCESS);
}

static int	process_token(t_command **cur, t_token **token_i)
{
	t_token	*t;
	int		r;

	t = *token_i;
	r = handle_simple_token(cur, t);
	if (r != ERROR)
		return (r);
	r = handle_redirection(*cur, token_i);
	return (r);
}

t_command	*build_commands_from_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = new_command();
			if (!current)
				return (NULL);
			if (!head)
				head = current;
		}
		if (process_token(&current, &tokens) == ERROR)
			return (NULL);
		tokens = tokens->next;
	}
	return (head);
}
