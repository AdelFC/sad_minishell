/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:44 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 16:02:41 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_outer_quotes(char *str, int *was_quoted)
{
	t_remove_quote_utils	rq;

	init_remove_quote_utils(&rq, str);
	if (!rq.out)
		return (NULL);
	*was_quoted = 0;
	remove_outer_quotes_loop(&rq, str, was_quoted);
	rq.out[rq.j] = '\0';
	free(str);
	if (*was_quoted && rq.last_quote)
		*was_quoted = rq.last_quote;
	return (rq.out);
}

static void	init_ppraw_utils(t_ppraw_utils *raw)
{
	raw->value = NULL;
	raw->was_quoted = 0;
	raw->key = NULL;
	raw->val = NULL;
	raw->type = 0;
	raw->token = NULL;
}

void	free_ppraw_utils(t_ppraw_utils *raw)
{
	if (raw->value)
		free(raw->value);
	if (raw->key)
		free(raw->key);
	if (raw->val)
		free(raw->val);
}

static int	handle_raw_operator(t_ppraw_utils *raw, t_token **tokens_out,
		t_raw *cur)
{
	raw->value = ft_strdup(cur->value);
	if (!raw->value)
	{
		free_ppraw_utils(raw);
		free_tokens(*tokens_out);
		return (ERROR);
	}
	raw->was_quoted = 0;
	return (SUCCESS);
}

int	post_process_raw(t_raw *raw_tokens, t_token **tokens_out)
{
	t_raw			*cur;
	t_ppraw_utils	raw;

	cur = raw_tokens;
	*tokens_out = NULL;
	while (cur)
	{
		init_ppraw_utils(&raw);
		if (cur->type == RAW_OPERATOR)
		{
			if (handle_raw_operator(&raw, tokens_out, cur) == ERROR)
				return (ERROR);
		}
		else
		{
			if (handle_assignment_or_quotes(&raw, cur) == ERROR)
				return (ERROR);
		}
		if (create_and_add_token(&raw, tokens_out) == ERROR)
			return (ERROR);
		free_ppraw_utils(&raw);
		cur = cur->next;
	}
	return (SUCCESS);
}
