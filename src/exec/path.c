/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:05:21 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 16:25:37 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_free_array(char **arr)
{
    int i = 0;

    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

/*
** Concatène dir + '/' + cmd en une nouvelle chaîne malloc'd.
*/
char *join_path(const char *dir, const char *cmd)
{
    size_t len_dir = ft_strlen(dir);
    size_t len_cmd = ft_strlen(cmd);
    char  *full = malloc(len_dir + 1 + len_cmd + 1);
    size_t i = 0;

    if (!full)
        return NULL;
    while (i < len_dir)
    {
        full[i] = dir[i];
        i++;
    }
    full[i++] = '/';
    memcpy(full + i, cmd, len_cmd);
    i += len_cmd;
    full[i] = '\0';
    return full;
}

/*
** Extrait la valeur de PATH dans envp et la split en tab de dirs.
*/
static char **get_path_dirs(char **envp)
{
    int     i = 0;
    char   *path_val;
    char   **dirs;

    while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp[i])
        return NULL;
    path_val = envp[i] + 5;
    dirs = ft_split(path_val, ':');
    return dirs;
}

/*
** Recherche cmd dans chaque répertoire de PATH.
** Si cmd contient '/', test direct via access().
** Renvoie un malloc'd path ou NULL si non trouvé.
*/
char *find_path(const char *cmd, char **envp)
{
    char   **dirs;
    char   *candidate;
    int     j;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    dirs = get_path_dirs(envp);
    if (!dirs)
        return NULL;
    j = 0;
    while (dirs[j])
    {
        candidate = join_path(dirs[j], cmd);
        if (candidate && access(candidate, X_OK) == 0)
        {
            ft_free_array(dirs);
            return candidate;
        }
        free(candidate);
        j++;
    }
    ft_free_array(dirs);
    return NULL;
}
