/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:20 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:30:06 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** env: sans arguments, affiche chaque VAR=VALUE.
** Si un argument est fourni :
**  - s’il commence par ‘-’, message ERR_ENV_OPTIONS
**  - sinon message ERR_ENV_ARG
** Retourne SUCCESS (0) ou ERROR (-1).
*/
int ft_env(char **argv, t_env **env)
{
    int i;
    t_env *tmp;

    i = 1;
    if (argv[i])
    {
        if (argv[i][0] == '-')
            ft_printf_error(ERR_ENV_OPTIONS);
        else
            ft_printf_error(ERR_ENV_ARG);
        return (ERROR);
    }
    tmp = *env;
    while (tmp)
    {
        if (tmp->name && tmp->value)
            printf("%s=%s\n", tmp->name, tmp->value);
        tmp = tmp->next;
    }
    return (SUCCESS);
}
