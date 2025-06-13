/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:17:25 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 15:30:02 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_double_quotes(t_expand_tok *v)
{
	v->res = strjoin_char(v->res, v->str[v->i++]);
	while (v->str[v->i] && v->str[v->i] != '"')
	{
		if (v->str[v->i] == '$')
			handle_dollar_sign(v, &v->res);
		else
			v->res = strjoin_char(v->res, v->str[v->i++]);
	}
	if (v->str[v->i] == '"')
		v->res = strjoin_char(v->res, v->str[v->i++]);
}

static int	expand_token_loop(t_expand_tok *v, t_token *token)
{
	while (v->str[v->i])
	{
		if (v->str[v->i] == '\'')
			handle_single_quotes(v->str, &v->res, &v->i);
		else if (v->str[v->i] == '"')
			handle_double_quotes(v);
		else if (v->str[v->i] == '$')
			handle_dollar_sign(v, &v->res);
		else
		{
			v->res = strjoin_char(v->res, v->str[v->i++]);
			if (!v->res)
			{
				free(token->value);
				token->value = NULL;
				return (ERROR);
			}
		}
	}
	return (SUCCESS);
}

void	expand_token_value(t_token *token, t_env *env_list, int last_status)
{
	t_expand_tok	v;

	init_expand_tok(&v, token->value, env_list, last_status);
	if (expand_token_loop(&v, token) == ERROR)
		return ;
	if (!v.res)
	{
		free(token->value);
		token->value = NULL;
		return ;
	}
	free(token->value);
	token->value = v.res;
}

void	expand_tokens(t_token **tokens, t_env *env_list, int last_status)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if ((current->type == T_ARG || current->type == T_CMD)
			&& current->was_quoted != 1)
			expand_token_value(current, env_list, last_status);
		current = current->next;
	}
}

void	handle_single_quotes(const char *str, char **result, int *i)
{
	*result = strjoin_char(*result, str[(*i)++]);
	if (!*result)
		return ;
	while (str[*i] && str[*i] != '\'')
	{
		*result = strjoin_char(*result, str[(*i)++]);
		if (!*result)
			return ;
	}
	if (str[*i])
	{
		*result = strjoin_char(*result, str[(*i)++]);
	}
}
