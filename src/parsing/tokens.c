/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:34:24 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 09:52:08 by afodil-c         ###   ########.fr       */
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

static t_token	*create_token(int type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		ft_printf_error("token error\n");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

static void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static int	new_token(char *value, t_token **tokens)
{
	t_token	*token;
	int		type;

	type = get_token_type(value, tokens);
	token = create_token(type, value);
	if (!token)
		return (ERROR);
	add_token(tokens, token);
	return (SUCCESS);
}

int	tokenize(char *line, t_token **tokens)
{
	int		i;
	char	*value;

	*tokens = NULL;
	i = 0;
	while (line[i] && is_space(line[i]) == SUCCESS)
		i++;
	while (line[i])
	{
		value = get_next_token(line, &i);
		if (!value)
			return (free_tokens(*tokens), ERROR);
		if (new_token(value, tokens) == ERROR)
			return (free(value), free_tokens(*tokens), ERROR);
		while (line[i] && is_space(line[i]) == SUCCESS)
			i++;
	}
	return (SUCCESS);
}
