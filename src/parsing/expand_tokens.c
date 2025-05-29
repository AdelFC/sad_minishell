/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 12:42:16 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar(t_expand_tok *v)
{
	char	*name;
	char	*val;
	char	*new;

	name = extract_var(v);
	val = get_env_value(v->env_list, name);
	if (!val && name && *name)
	{
		new = build_new_res(v->res, name);
		free(name);
		if (!new)
			return ;
		free(v->res);
		v->res = new;
		return ;
	}
	free(name);
	if (!val)
		val = "";
	new = build_new_res(v->res, val);
	if (!new)
		return ;
	free(v->res);
	v->res = new;
}

static void	process_char(t_expand_tok *v)
{
	char	c;
	char	next;

	c = v->str[v->i];
	if (c == '\'' && !v->in_dq)
		v->in_sq = !v->in_sq;
	else if (c == '"' && !v->in_sq)
		v->in_dq = !v->in_dq;
	else if (c == '$' && !v->in_sq)
	{
		next = v->str[v->i + 1];
		if (next == '?' || ft_isalnum(next) || next == '_')
		{
			v->i++;
			handle_dollar(v);
			return ;
		}
		append_char(v, '$');
	}
	else
		append_char(v, c);
	v->i++;
}

char	*expand_string(const char *s, t_env *env_list, int last_status)
{
	t_expand_tok	v;
	char			*result;

	init_expand_tok(&v, s, env_list, last_status);
	if (!v.res)
		return (NULL);
	while (v.str[v.i])
		process_char(&v);
	result = v.res;
	return (result);
}

void	build_tokens(t_split_utils *u, t_token *tok)
{
	t_token	*n;

	while (u->words[u->i])
	{
		n = malloc(sizeof(*n));
		if (!n)
			break ;
		ft_memset(n, 0, sizeof(*n));
		if (u->i == 0)
			n->type = tok->type;
		else
			n->type = T_ARG;
		n->value = ft_strdup(u->words[u->i]);
		n->was_quoted = tok->was_quoted;
		if (u->last)
			u->last->next = n;
		else
			u->first = n;
		u->last = n;
		u->i++;
	}
}

void	expand_tokens(t_token **head, t_env *env_list, int last_status)
{
	t_token	*tok;

	tok = *head;
	while (tok)
	{
		if (is_expandable_token(tok))
			tok = process_expansion(head, tok, env_list, last_status);
		else
			tok = tok->next;
	}
}
