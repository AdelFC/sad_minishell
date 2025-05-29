/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:33:57 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 11:10:47 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(const char *line, int *i)
{
	int		start;
	char	op;

	start = *i;
	op = line[*i];
	if (op == '>' || op == '<')
	{
		if (line[*i + 1] == op)
			*i += 2;
		else
			(*i)++;
	}
	else if (op == '|')
		(*i)++;
	else
		return (NULL);
	return (ft_substr(line, start, *i - start));
}

static char	*extract_quoted(const char *line, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i] != quote)
		return (NULL);
	(*i)++;
	return (ft_substr(line, start, *i - start));
}

static char	*extract_word(const char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && is_space(line[*i]) == ERROR
		&& is_operator(line[*i]) == ERROR)
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

static char	*extract_segment(const char *line, int *i)
{
	if (line[*i] == '\'' || line[*i] == '"')
		return (extract_quoted(line, i));
	return (extract_word(line, i));
}

char	*get_next_token(const char *line, int *i)
{
	char	*token_value;
	char	*seg;
	int		start;
	int		j;
	int		k;
	char	quote;

	start = *i;
	while (line[*i] && is_space(line[*i]) == SUCCESS)
		(*i)++;
	if (line[*i] && is_operator(line[*i]) == SUCCESS)
		return (extract_operator(line, i));
	j = *i;
	while (line[j] && line[j] != '=' && is_space(line[j]) == ERROR
		&& is_operator(line[j]) == ERROR)
		j++;
	if (line[j] == '=' && (line[j + 1] == '"' || line[j + 1] == '\''))
	{
		quote = line[j + 1];
		k = j + 2;
		while (line[k] && line[k] != quote)
			k++;
		if (line[k] == quote)
		{
			k++;
			*i = k;
			return (ft_substr(line, start, k - start));
		}
	}
	token_value = NULL;
	while (line[*i] && is_space(line[*i]) == ERROR
		&& is_operator(line[*i]) == ERROR)
	{
		seg = extract_segment(line, i);
		if (!seg)
			return (free(token_value), NULL);
		if (!token_value)
			token_value = seg;
		else
			token_value = join_and_free(token_value, seg);
		if (!token_value)
			return (NULL);
	}
	if (!token_value)
		token_value = ft_strdup("");
	return (token_value);
}
