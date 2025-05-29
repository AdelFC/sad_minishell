/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:33:57 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 12:35:29 by barnaud          ###   ########.fr       */
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

char	*extract_quoted(const char *line, int *i)
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

char	*extract_word(const char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && is_space(line[*i]) == ERROR
		&& is_operator(line[*i]) == ERROR)
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	*extract_segment(const char *line, int *i)
{
	if (line[*i] == '\'' || line[*i] == '"')
		return (extract_quoted(line, i));
	return (extract_word(line, i));
}

char	*get_next_token(const char *line, int *i)
{
	int		start;
	int		k;
	int		j;
	char	quote;

	start = *i;
	skip_spaces2(line, i);
	if (line[*i] && is_operator(line[*i]) == SUCCESS)
		return (extract_operator(line, i));
	j = find_assignment_with_quote(line, *i, &k, &quote);
	if (j != -1)
	{
		k++;
		*i = k;
		return (ft_substr(line, start, k - start));
	}
	return (extract_token_value(line, i));
}
