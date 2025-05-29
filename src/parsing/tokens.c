/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:34:24 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 11:57:45 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*cur;
	t_token	*next;

	cur = tokens;
	while (cur)
	{
		next = cur->next;
		free(cur->value);
		free(cur);
		cur = next;
	}
}

static char	*handle_eq_quote_case(char *value, int *was_quoted)
{
	t_get_token_utils	u;

	u.eq = ft_strchr(value, '=');
	if (u.eq && (u.eq[1] == '\'' || u.eq[1] == '"'))
	{
		u.quote = u.eq[1];
		u.len = ft_strlen(u.eq + 1);
		if (u.len > 2 && u.eq[1 + u.len - 1] == u.quote)
		{
			*was_quoted = 1;
			u.before = ft_substr(value, 0, (u.eq - value) + 1);
			u.stripped = ft_substr(u.eq + 2, 0, u.len - 2);
			u.joined = join_and_free(u.before, u.stripped);
			free(value);
			if (!u.joined)
				return (NULL);
			return (u.joined);
		}
	}
	return (NULL);
}

static char	*get_clean_token(char *line, int *i, int *was_quoted)
{
	char	*value;
	char	*result;

	*was_quoted = 0;
	value = get_next_token(line, i);
	if (!value)
		return (NULL);
	result = handle_eq_quote_case(value, was_quoted);
	if (result)
		return (result);
	if ((value[0] == '\'' && value[ft_strlen(value) - 1] == '\''
			&& ft_strlen(value) > 1) || (value[0] == '"'
			&& value[ft_strlen(value) - 1] == '"' && ft_strlen(value) > 1))
	{
		*was_quoted = 1;
		result = ft_substr(value, 1, ft_strlen(value) - 2);
		free(value);
		if (result)
			return (result);
		else
			return (NULL);
	}
	return (value);
}

int	tokenize(char *line, t_token **tokens)
{
	int		i;
	int		was_quoted;
	char	*value;

	*tokens = NULL;
	i = 0;
	while (line[i] && is_space(line[i]) == SUCCESS)
		i++;
	while (line[i])
	{
		value = get_clean_token(line, &i, &was_quoted);
		if (!value)
			return (free_tokens(*tokens), ERROR);
		if (new_token(value, was_quoted, tokens) == ERROR)
			return (free(value), free_tokens(*tokens), ERROR);
		while (line[i] && is_space(line[i]) == SUCCESS)
			i++;
	}
	return (SUCCESS);
}
