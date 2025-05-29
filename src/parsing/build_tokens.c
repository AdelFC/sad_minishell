/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:54:55 by barnaud           #+#    #+#             */
/*   Updated: 2025/05/29 11:58:07 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_token(int type, char *value, int was_quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		ft_printf_error("token error\n");
		exit(EXIT_FAILURE);
	}
	ft_memset(token, 0, sizeof(t_token));
	token->type = type;
	token->value = value;
	token->was_quoted = was_quoted;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
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

int	new_token(char *value, int was_quoted, t_token **tokens)
{
	t_token	*token;
	int		type;

	type = get_token_type(value, tokens);
	token = create_token(type, value, was_quoted);
	if (!token)
		return (ERROR);
	add_token(tokens, token);
	return (SUCCESS);
}
