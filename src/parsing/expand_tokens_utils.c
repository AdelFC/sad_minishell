/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:26:23 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 09:58:17 by afodil-c         ###   ########.fr       */
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

char	*extract_var(t_expand_tok *v)
{
	int	start;

	start = v->i;
	if (v->str[v->i] == '?')
	{
		v->i++;
		return (ft_itoa(v->last_status));
	}
	while (v->str[v->i] && (ft_isalnum(v->str[v->i]) || v->str[v->i] == '_'))
		v->i++;
	return (ft_substr(v->str + start, 0, v->i - start));
}

char	*get_env_value(t_env *env_list, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	while (env_list)
	{
		if (!ft_strncmp(env_list->name, key, len)
			&& env_list->name[len] == '\0')
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*build_new_res(const char *old, const char *val)
{
	size_t	r;
	size_t	v;
	char	*buf;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	r = ft_strlen(old);
	v = ft_strlen(val);
	buf = malloc(r + v + 1);
	if (!buf)
		return (NULL);
	while (i < r)
	{
		buf[i] = old[i];
		i++;
	}
	while (j < v)
	{
		buf[i + j] = val[j];
		j++;
	}
	buf[i + j] = '\0';
	return (buf);
}

void	append_char(t_expand_tok *v, char c)
{
	size_t	len;
	char	*buf;
	size_t	i;

	len = ft_strlen(v->res);
	buf = malloc(len + 2);
	if (!buf)
		return ;
	i = 0;
	while (i < len)
	{
		buf[i] = v->res[i];
		i++;
	}
	buf[i++] = c;
	buf[i] = '\0';
	free(v->res);
	v->res = buf;
}
