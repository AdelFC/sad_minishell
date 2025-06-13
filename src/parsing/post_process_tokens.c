/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:44 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 15:15:57 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_remove_quote_utils(t_remove_quote_utils *rq, char *str)
{
	rq->in_sq = 0;
	rq->in_dq = 0;
	rq->i = 0;
	rq->j = 0;
	rq->last_quote = 0;
	rq->len = ft_strlen(str);
	rq->out = malloc(rq->len + 1);
}

int	split_assignment_quoted(const char *raw_value, char **assign_key,
		char **assign_val, int *was_quoted)
{
	int		i;
	int		len;
	char	quote;
	int		j;

	i = 0;
	len = ft_strlen(raw_value);
	while (i < len && raw_value[i] != '=')
		i++;
	if (i == len || i + 1 >= len)
		return (ERROR);
	quote = raw_value[i + 1];
	if (quote != '\'' && quote != '"')
		return (ERROR);
	j = i + 2;
	while (j < len && raw_value[j] != quote)
		j++;
	if (j >= len || raw_value[j] != quote)
		return (ERROR);
	*assign_key = ft_substr(raw_value, 0, i + 1);
	if (!*assign_key)
		return (ERROR);
	*assign_val = ft_substr(raw_value, i + 2, j - (i + 2));
	if (!*assign_val)
	{
		free(*assign_key);
		return (ERROR);
	}
	*was_quoted = 1;
	return (SUCCESS);
}

char	*remove_outer_quotes(char *str, int *was_quoted)
{
	t_remove_quote_utils	rq;

	init_remove_quote_utils(&rq, str);
	if (!rq.out)
		return (NULL);
	*was_quoted = 0;
	while (str[rq.i])
	{
		if (str[rq.i] == '\'' && !rq.in_dq)
		{
			rq.in_sq = !rq.in_sq;
			rq.last_quote = 1;
			*was_quoted = 1;
			rq.i++;
		}
		else if (str[rq.i] == '"' && !rq.in_sq)
		{
			rq.in_dq = !rq.in_dq;
			rq.last_quote = 2;
			*was_quoted = 2;
			rq.i++;
		}
		else
			rq.out[rq.j++] = str[rq.i++];
	}
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

static void	free_ppraw_utils(t_ppraw_utils *raw)
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

static int	handle_assignment_or_quotes(t_ppraw_utils *raw, t_raw *cur)
{
	char	*tmp;

	if (split_assignment_quoted(cur->value, &raw->key, &raw->val,
			&raw->was_quoted) == SUCCESS)
	{
		raw->value = join_and_free(raw->key, raw->val);
		if (!raw->value)
		{
			free_ppraw_utils(raw);
			return (ERROR);
		}
		raw->key = NULL;
		raw->val = NULL;
	}
	else
	{
		raw->value = ft_strdup(cur->value);
		if (!raw->value)
		{
			free_ppraw_utils(raw);
			return (ERROR);
		}
		tmp = remove_outer_quotes(raw->value, &raw->was_quoted);
		if (!tmp)
		{
			free(raw->value);
			free_ppraw_utils(raw);
			return (ERROR);
		}
		raw->value = tmp;
	}
	return (SUCCESS);
}

static int	create_and_add_token(t_ppraw_utils *raw, t_token **tokens_out)
{
	if (raw->was_quoted)
		raw->type = T_ARG;
	else
		raw->type = get_token_type(raw->value, tokens_out);
	raw->token = create_token(raw->type, raw->value, raw->was_quoted);
	if (!raw->token)
	{
		free(raw->value);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	if (raw->type == T_ARG && raw->value == NULL)
	{
		free(raw->token->value);
		free(raw->token);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	if (!tokens_out || !raw->token)
	{
		free(raw->token->value);
		free(raw->token);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	add_token(tokens_out, raw->token);
	raw->value = NULL;
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
