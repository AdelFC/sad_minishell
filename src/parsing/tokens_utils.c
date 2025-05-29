/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:29:51 by barnaud           #+#    #+#             */
/*   Updated: 2025/05/29 12:35:09 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces2(const char *line, int *i)
{
	while (line[*i] && is_space(line[*i]) == SUCCESS)
		(*i)++;
}

int	find_equal_sign(const char *line, int i)
{
	while (line[i] && line[i] != '=' && is_space(line[i]) == ERROR
		&& is_operator(line[i]) == ERROR)
		i++;
	return (i);
}

int	check_assignment_quote(const char *line, int j, int *k, char *quote)
{
	if (line[j] == '=' && (line[j + 1] == '"' || line[j + 1] == '\''))
	{
		*quote = line[j + 1];
		*k = j + 2;
		while (line[*k] && line[*k] != *quote)
			(*k)++;
		if (line[*k] == *quote)
			return (1);
	}
	return (0);
}

int	find_assignment_with_quote(const char *line, int i, int *k,
		char *quote)
{
	int	j;

	j = find_equal_sign(line, i);
	if (check_assignment_quote(line, j, k, quote))
		return (j);
	return (-1);
}

char	*extract_token_value(const char *line, int *i)
{
	char	*token_value;
	char	*seg;

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
