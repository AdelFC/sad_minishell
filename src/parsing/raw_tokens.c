/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:50:49 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/10 21:01:18 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	raw_tokenize_loop(const char *line, t_raw_utils *raw)
{
	char		*value;
	t_raw_type	type;

	while (line[raw->i])
	{
		if (is_operator(line[raw->i]) == SUCCESS)
		{
			value = extract_raw_operator(line, &raw->i);
			type = RAW_OPERATOR;
		}
		else
		{
			value = extract_raw_token(line, &raw->i);
			type = RAW_WORD;
		}
		if (!value)
			return (free_raw_tokens(raw->head), ERROR);
		if (append_raw(&raw->head, &raw->last, value, type) == ERROR)
			return (free(value), free_raw_tokens(raw->head), ERROR);
		while (line[raw->i] && is_space(line[raw->i]) == SUCCESS)
			raw->i++;
	}
	return (SUCCESS);
}

int	raw_tokenize(const char *line, t_raw **raw_tokens)
{
	t_raw_utils	raw;
	int			status;

	raw.head = NULL;
	raw.last = NULL;
	raw.i = 0;
	while (line[raw.i] && is_space(line[raw.i]) == SUCCESS)
		raw.i++;
	status = raw_tokenize_loop(line, &raw);
	if (status == ERROR)
		return (ERROR);
	*raw_tokens = raw.head;
	return (SUCCESS);
}
