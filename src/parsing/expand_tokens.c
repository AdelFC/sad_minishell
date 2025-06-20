/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:13:57 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/20 10:12:10 by afodil-c         ###   ########.fr       */
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

static void	handle_dollar_special(t_expand_tok *v, char **result)
{
	*result = strjoin_char(*result, '$');
	if (v->str[v->i])
	{
		*result = strjoin_char(*result, v->str[v->i]);
		v->i++;
	}
}

static char	*get_var_value(t_expand_tok *v)
{
	char	*var_name;
	char	*var_value;
	int		start;

	var_name = NULL;
	var_value = NULL;
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
	return (var_value);
}

void	handle_dollar_sign(t_expand_tok *v, char **result)
{
	int		cond;
	char	*var_value;
	char	*to_join;
	int		allocated;

	to_join = NULL;
	allocated = 0;
	v->i++;
	cond = v->str[v->i] == '?';
	var_value = get_var_value(v);
	if (!var_value && !(ft_isalpha(v->str[v->i - 1]) || v->str[v->i - 1] == '_'
			|| v->str[v->i - 1] == '?'))
	{
		handle_dollar_special(v, result);
		return ;
	}
	to_join = get_to_join(var_value, &allocated);
	if (!to_join)
		return ;
	*result = join_and_free(*result, to_join);
	if (cond || (!var_value && allocated))
		free(var_value);
}
