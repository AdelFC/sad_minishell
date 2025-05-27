/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:36:32 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 23:36:35 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get(t_env *env, const char *name)
{
	size_t	len;

	len = ft_strlen(name);
	while (env)
	{
		if (!ft_strncmp(env->name, name, len + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	update_env_value(t_env *cur, const char *value)
{
	free(cur->value);
	cur->value = ft_strdup(value);
	if (!cur->value)
		return (ERROR);
	return (SUCCESS);
}

int	env_set(t_env **env, const char *name, const char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = *env;
	while (cur)
	{
		if (!ft_strncmp(cur->name, name, ft_strlen(name) + 1))
			return (update_env_value(cur, value));
		if (!cur->next)
			break ;
		cur = cur->next;
	}
	new = new_env(name, value);
	if (!new)
		return (ERROR);
	if (cur)
		cur->next = new;
	else
		*env = new;
	return (SUCCESS);
}
