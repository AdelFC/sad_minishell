/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 11:17:52 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 11:34:25 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_list_size(t_env *env)
{
	int		size;
	t_env	*cur;

	size = 0;
	cur = env;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return (size);
}

static int	fill_env_array(char **array, t_env *env)
{
	t_env	*cur;
	char	*tmp;
	int		i;

	cur = env;
	i = 0;
	while (cur)
	{
		tmp = ft_strjoin_nobug(cur->name, "=");
		array[i] = join_and_free(tmp, ft_strjoin_nobug("", cur->value));
		if (!array[i])
		{
			while (i--)
				free(array[i]);
			return (0);
		}
		i++;
		cur = cur->next;
	}
	array[i] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env)
{
	char	**array;
	int		size;

	size = env_list_size(env);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	if (!fill_env_array(array, env))
	{
		free(array);
		return (NULL);
	}
	return (array);
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
