/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extract_raw.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:03:00 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/12 15:08:45 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_raw_word_segment(const char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && (is_space(line[*i]) == ERROR)
		&& (is_operator(line[*i]) == ERROR)
		&& (line[*i] != '\'') && (line[*i] != '"'))
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	*extract_raw_quoted(const char *line, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i] == quote)
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	*extract_raw_token(const char *line, int *i)
{
	char	*res;
	char	*seg;
	char	*tmp;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (line[*i] && (is_space(line[*i]) == ERROR)
		&& (is_operator(line[*i]) == ERROR))
	{
		if (line[*i] == '\'' || line[*i] == '"')
			seg = extract_raw_quoted(line, i);
		else
			seg = extract_raw_word_segment(line, i);
		if (!seg)
		{
			free(res);
			return (NULL);
		}
		tmp = join_and_free(res, seg);
		if (!tmp)
			return (free(res), free(seg), NULL);
		res = tmp;
	}
	return (res);
}

char	*extract_raw_operator(const char *line, int *i)
{
	int		start;
	char	op;

	start = *i;
	op = line[*i];
	if (op == '>' || op == '<')
	{
		if (line[start + 1] == op)
			*i = start + 2;
		else
			*i = start + 1;
	}
	else if (op == '|')
		*i = start + 1;
	else
		return (NULL);
	return (ft_substr(line, start, *i - start));
}
