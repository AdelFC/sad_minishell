/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:13:57 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/12 20:55:09 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_expand_tok(t_expand_tok *v, const char *s, t_env *env_list,
			int last_status)
{
	v->str = s;
	v->env_list = env_list;
	v->last_status = last_status;
	v->i = 0;
	v->in_sq = 0;
	v->in_dq = 0;
	v->res = ft_strdup("");
}

char	*strjoin_char(char *str, char c)
{
	size_t	len;
	char	*result;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
	{
		free(str);
		return (NULL);
	}
	if (str)
		ft_memcpy(result, str, len);
	result[len] = c;
	result[len + 1] = '\0';
	free(str);
	return (result);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->name, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
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

void	handle_dollar_sign(t_expand_tok *v, char **result)
{
	char	*var_name;
	char	*var_value;
	int		start;

	var_name = NULL;
	var_value = NULL;
	v->i++;
	if (v->str[v->i] == '?')
	{
		var_value = ft_itoa(v->last_status);
		v->i++;
	}
	else if (ft_isalpha(v->str[v->i]) || v->str[v->i] == '_')
	{
		start = v->i;
		while (v->str[v->i] && (ft_isalnum(v->str[v->i])
				|| v->str[v->i] == '_'))
			v->i++;
		var_name = ft_substr(v->str, start, v->i - start);
		var_value = get_env_value(v->env_list, var_name);
		free(var_name);
	}
	else
	{
		*result = strjoin_char(*result, '$');
		if (v->str[v->i])
		{
			*result = strjoin_char(*result, v->str[v->i]);
			v->i++;
		}
		return ;
	}
	if (!var_value)
		var_value = ft_strdup("");
	*result = join_and_free(*result, ft_strdup(var_value));
}

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

void	expand_token_value(t_token *token, t_env *env_list, int last_status)
{
    t_expand_tok	v;

    init_expand_tok(&v, token->value, env_list, last_status);
    while (v.str[v.i])
    {
        if (v.str[v.i] == '\'')
            handle_single_quotes(v.str, &v.res, &v.i);
        else if (v.str[v.i] == '"')
            handle_double_quotes(&v);
        else if (v.str[v.i] == '$')
            handle_dollar_sign(&v, &v.res);
        else
        {
            v.res = strjoin_char(v.res, v.str[v.i++]);
            if (!v.res)
    		{
    		    free(token->value);
    		    token->value = NULL;
    		    return;
    		}
        }
    }
    if (!v.res)
    {
        free(token->value);
        token->value = NULL;
        return;
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
