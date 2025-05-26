/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:17 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:43:24 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/*
** Handle cd errors and decide the path to use.
** On success: *path_out points to argv[1] or HOME; return SUCCESS.
** On error: prints message and return (ERROR).
*/
static int handle_cd_errors(char **argv, t_env **env, char **path_out)
{
    if (!argv[1])
    {
        *path_out = env_get(*env, "HOME");
        if (!*path_out)
        {
            ft_printf_error(ERR_CD_HOME_NOT_SET);
            return (ERROR);
        }
    }
    else if (argv[2])
    {
        ft_printf_error(ERR_CD_TOO_MANY_ARGS);
        return (ERROR);
    }
    else
    {
        *path_out = argv[1];
    }
    return (SUCCESS);
}

/*
** cd builtin:
** - argv[0] == "cd"
** - argv[1] is target or NULL
** - uses handle_cd_errors(), chdir(), and updates OLDPWD/PWD
*/
int ft_cd(char **argv, t_env **env)
{
    char *oldcwd;
    char *newcwd;
    char *target;

    if (handle_cd_errors(argv, env, &target) == ERROR)
        return (ERROR);
    oldcwd = getcwd(NULL, 0);
    if (!oldcwd)
    {
        ft_printf_error(ERR_CD_CWD);
        return (ERROR);
    }
    if (chdir(target) != 0)
    {
        ft_printf_error(ERR_CD_NO_SUCH_DIR, target);
        free(oldcwd);
        return (ERROR);
    }
    if (env_set(env, "OLDPWD", oldcwd) == ERROR)
    {
        free(oldcwd);
        return (ERROR);
    }
    free(oldcwd);
    newcwd = getcwd(NULL, 0);
    if (!newcwd)
        return (ERROR);
    if (env_set(env, "PWD", newcwd) == ERROR)
    {
        free(newcwd);
        return (ERROR);
    }
    free(newcwd);
    return (SUCCESS);
}
