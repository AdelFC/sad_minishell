/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:05:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 10:03:04 by afodil-c         ###   ########.fr       */
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

static char	**get_path_dirs(char **envp)
{
	int		i;
	char	*path_val;
	char	**dirs;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path_val = envp[i] + 5;
	dirs = ft_split(path_val, ':');
	return (dirs);
}

char	*find_path(const char *cmd, char **envp)
{
	char	**dirs;
	char	*candidate;
	int		j;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	dirs = get_path_dirs(envp);
	if (!dirs)
		return (NULL);
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
