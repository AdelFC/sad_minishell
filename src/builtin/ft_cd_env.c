/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:41:11 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:46:32 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Retrieve the value of environment variable NAME from env list.
** Returns NULL if not found.
*/
char *env_get(t_env *env, const char *name)
{
    size_t len = ft_strlen(name);
    while (env)
    {
        if (!ft_strncmp(env->name, name, len + 1))
            return env->value;
        env = env->next;
    }
    return NULL;
}

/*
** Set or update environment variable NAME to VALUE in *env.
** Returns SUCCESS or ERROR on allocation failure.
*/
int env_set(t_env **env, const char *name, const char *value)
{
    t_env *cur = *env;
    t_env *new;

    while (cur)
    {
        if (!ft_strncmp(cur->name, name, ft_strlen(name) + 1))
        {
            free(cur->value);
            cur->value = ft_strdup(value);
            if (!cur->value)
                return ERROR;
            return SUCCESS;
        }
        if (!cur->next)
            break;
        cur = cur->next;
    }
    new = new_env(name, value);
    if (!new)
        return ERROR;
    if (cur)
        cur->next = new;
    else
        *env = new;
    return SUCCESS;
}
