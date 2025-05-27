/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 21:52:44 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 22:06:17 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_env_content(t_env *a, t_env *b)
{
	char	*tmp_name;
	char	*tmp_value;

	tmp_name = a->name;
	tmp_value = a->value;
	a->name = b->name;
	a->value = b->value;
	b->name = tmp_name;
	b->value = tmp_value;
}

t_env	*sort_env_alpha(t_env *env)
{
	t_sort_utils	u;

	if (!env)
		return (env);
	u.swapped = 1;
	while (u.swapped)
	{
		u.swapped = 0;
		u.curr = env;
		while (u.curr->next)
		{
			u.len = ft_strlen(u.curr->name);
			if (ft_strncmp(u.curr->name, u.curr->next->name, u.len + 1) > 0)
			{
				swap_env_content(u.curr, u.curr->next);
				u.swapped = 1;
			}
			u.curr = u.curr->next;
		}
	}
	return (env);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
		return (ERROR);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

char	*get_var_name(const char *arg)
{
	size_t	pos;
	char	*name;

	pos = 0;
	while (arg[pos] && arg[pos] != '=')
		pos++;
	name = ft_substr(arg, 0, pos);
	return (name);
}

int	export_with_value(const char *arg, t_env **env)
{
	char	*name;
	char	*value;
	int		ret;

	name = get_var_name(arg);
	if (!name)
		return (ERROR);
	value = ft_strdup(arg + ft_strlen(name) + 1);
	if (!value)
	{
		free(name);
		return (ERROR);
	}
	ret = env_set(env, name, value);
	free(name);
	free(value);
	return (ret);
}
