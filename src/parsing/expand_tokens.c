/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/25 23:39:16 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/*
** Gère l'expansion d'une variable
*/
static void handle_dollar(t_expand_tok *v)
{
    char *name = extract_var(v);
    char *raw  = get_env_value(v->env_list, name);
    char *dup  = ft_strdup(raw ? raw : "");
    char *new;

    free(name);
    if (!dup) return;

    new = build_new_res(v->res, dup);
    free(dup);
    if (!new) return;

    free(v->res);
    v->res = new;
}

/*
** Traite un caractère selon le contexte
*/
static void process_char(t_expand_tok *v)
{
    char c = v->str[v->i];

    if (c == '\'' && !v->in_dq)
    {
        v->in_sq = !v->in_sq;
        v->i++;
    }
    else if (c == '"' && !v->in_sq)
    {
        v->in_dq = !v->in_dq;
        v->i++;
    }
    else if (c == '$' && !v->in_sq)
    {
        v->i++;
        handle_dollar(v);
    }
    else
    {
        append_char(v, c);
        v->i++;
    }
}

/*
** Développe une chaîne
*/
char *expand_string(const char *s, t_env *env_list, int last_status)
{
    t_expand_tok v;
    char *result;

    init_expand_tok(&v, s, env_list, last_status);
    if (!v.res) return NULL;

    while (v.str[v.i])
        process_char(&v);

    result = v.res;
    return result;
}

/*
** Applique expansion sur chaque token texte
*/
void expand_tokens(t_token *tok, t_env *env_list, int last_status)
{
    while (tok)
    {
        if (tok->type != T_PIPE
         && tok->type != T_INFILE_OPERATOR
         && tok->type != T_OUTFILE_OPERATOR
         && tok->type != T_APPEND_OPERATOR
         && tok->type != T_HEREDOC_OPERATOR)
        {
            char *old = tok->value;
            char *new = expand_string(old, env_list, last_status);
            if (new)
            {
                free(old);
                tok->value = new;
            }
        }
        tok = tok->next;
    }
}
