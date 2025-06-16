/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:05:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 12:42:40 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*join_path(const char *dir, const char *cmd)
{
	size_t	len_dir;
	size_t	len_cmd;
	char	*full;
	size_t	i;

	len_dir = ft_strlen(dir);
	len_cmd = ft_strlen(cmd);
	full = malloc(len_dir + 1 + len_cmd + 1);
	i = 0;
	if (!full)
		return (NULL);
	while (i < len_dir)
	{
		full[i] = dir[i];
		i++;
	}
	full[i++] = '/';
	memcpy(full + i, cmd, len_cmd);
	i += len_cmd;
	full[i] = '\0';
	return (full);
}

static char	*get_env_v(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strncmp(env->name, name, ft_strlen(name) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*search_in_path(const char *cmd, char **dirs)
{
	char	*candidate;
	int		j;

	j = 0;
	while (dirs[j])
	{
		candidate = join_path(dirs[j], cmd);
		if (candidate && access(candidate, X_OK) == 0)
		{
			ft_free_array(dirs);
			return (candidate);
		}
		free(candidate);
		j++;
	}
	ft_free_array(dirs);
	return (NULL);
}

char	*find_path(const char *cmd, t_env *env)
{
	char	**dirs;
	char	*path_val;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_val = get_env_v(env, "PATH");
	if (!path_val)
		return (NULL);
	dirs = ft_split(path_val, ':');
	if (!dirs)
		return (NULL);
	return (search_in_path(cmd, dirs));
}
