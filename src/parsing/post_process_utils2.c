/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:02:56 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 16:03:11 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_remove_quote_utils(t_remove_quote_utils *rq, char *str)
{
	rq->in_sq = 0;
	rq->in_dq = 0;
	rq->i = 0;
	rq->j = 0;
	rq->last_quote = 0;
	rq->len = ft_strlen(str);
	rq->out = malloc(rq->len + 1);
}

int	get_assignment_indices(const char *raw_value, int *eq_idx,
		int *quote_idx, char *quote)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	len = ft_strlen(raw_value);
	while (i < len && raw_value[i] != '=')
		i++;
	if (i == len || i + 1 >= len)
		return (ERROR);
	*quote = raw_value[i + 1];
	if (*quote != '\'' && *quote != '"')
		return (ERROR);
	j = i + 2;
	while (j < len && raw_value[j] != *quote)
		j++;
	if (j >= len || raw_value[j] != *quote)
		return (ERROR);
	*eq_idx = i;
	*quote_idx = j;
	return (SUCCESS);
}

int	split_assignment_quoted(const char *raw_value, char **assign_key,
		char **assign_val, int *was_quoted)
{
	int		eq_idx;
	int		quote_idx;
	char	quote;

	if (get_assignment_indices(raw_value, &eq_idx, &quote_idx, &quote) == ERROR)
		return (ERROR);
	*assign_key = ft_substr(raw_value, 0, eq_idx + 1);
	if (!*assign_key)
		return (ERROR);
	*assign_val = ft_substr(raw_value, eq_idx + 2, quote_idx - (eq_idx + 2));
	if (!*assign_val)
	{
		free(*assign_key);
		return (ERROR);
	}
	*was_quoted = 1;
	return (SUCCESS);
}

void	remove_outer_quotes_loop(t_remove_quote_utils *rq, char *str,
		int *was_quoted)
{
	while (str[rq->i])
	{
		if (str[rq->i] == '\'' && !rq->in_dq)
		{
			rq->in_sq = !rq->in_sq;
			rq->last_quote = 1;
			*was_quoted = 1;
			rq->i++;
		}
		else if (str[rq->i] == '"' && !rq->in_sq)
		{
			rq->in_dq = !rq->in_dq;
			rq->last_quote = 2;
			*was_quoted = 2;
			rq->i++;
		}
		else
			rq->out[rq->j++] = str[rq->i++];
	}
}