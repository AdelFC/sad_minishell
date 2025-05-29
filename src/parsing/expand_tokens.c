/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 11:34:15 by afodil-c         ###   ########.fr       */
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

static t_token **find_pp(t_token **head, t_token *tok)
{
    t_token **pp = head;

    while (*pp && *pp != tok)
        pp = &(*pp)->next;
    return (pp);
}

static void build_tokens(t_split_utils *u, t_token *tok)
{
    t_token *n;

    while (u->words[u->i])
    {
        n = malloc(sizeof(*n));
        if (!n)
            break;
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

static void free_words(char **words)
{
    int i;

    i = 0;
    while (words[i])
        free(words[i++]);
    free(words);
}

static t_token *split_and_insert_tokens(t_token **head,	t_token *tok, char *str)
{
    t_split_utils u;

    u.words = ft_split(str, ' ');
    if (!u.words)
        return (NULL);
    u.pp = find_pp(head, tok);
    u.first = NULL;
    u.last = NULL;
    u.i = 0;
    build_tokens(&u, tok);
    if (u.last)
    {
        u.last->next = tok->next;
        *(u.pp)      = u.first;
    }
    free(tok->value);
    free(tok);
    free_words(u.words);
    if (u.last)
        return (u.last->next);
    return (NULL);
}

void	expand_tokens(t_token **head, t_env *env_list, int last_status)
{
	t_token *tok;

	tok = *head;
	while (tok)
	{
		if (tok->type != T_PIPE && tok->type != T_INFILE_OPERATOR
			&& tok->type != T_OUTFILE_OPERATOR && tok->type != T_APPEND_OPERATOR
			&& tok->type != T_HEREDOC_OPERATOR)
		{
			char *old = tok->value;
			char *new = expand_string(old, env_list, last_status);
			if (new)
			{
				if (!tok->was_quoted && ft_strchr(new, ' '))
				{
					tok = split_and_insert_tokens(head, tok, new);
					free(new);
					continue ;
				}
				free(old);
				tok->value = new;
			}
		}
		tok = tok->next;
	}
}
