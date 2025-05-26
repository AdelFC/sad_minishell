/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:05 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:36:48 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Supprime la variable d'env nommée name de la liste env_list.
** Si la variable n'existe pas, renvoie ERROR, sinon SUCCESS.
*/
static int remove_env_var(t_env **env_list, const char *name)
{
    t_env *cur = *env_list;
    t_env *prev = NULL;

    while (cur)
    {
        if (!ft_strncmp(cur->name, name, ft_strlen(name) + 1))
        {
            if (prev)
                prev->next = cur->next;
            else
                *env_list = cur->next;
            free(cur->name);
            free(cur->value);
            free(cur);
            return (SUCCESS);
        }
        prev = cur;
        cur = cur->next;
    }
    return (ERROR);
}

/*
** unset: retire une ou plusieurs variables d'environnement.
** - argv[1..] sont les noms à supprimer.
** - Si aucun argument, ERREUR.
** - Pour chaque nom, si introuvable, affiche ERR_UNSET_NOT_FOUND.
** Retourne SUCCESS si au moins une suppression valide, sinon ERROR.
*/
int ft_unset(char **argv, t_env **env)
{
    int i;
    int status = SUCCESS;
    int res;

    i = 1;
    if (!argv[i])
    {
        ft_printf_error(ERR_UNSET_NO_ARGS);
        return ERROR;
    }
    while (argv[i])
    {
        if (!argv[i][0] || !(ft_isalpha(argv[i][0]) || argv[i][0] == '_'))
        {
            ft_printf_error(ERR_EXPORT_VALID_ID, argv[i]);
            status = ERROR;
        }
        else
        {
            res = remove_env_var(env, argv[i]);
            if (res == ERROR)
            {
                ft_printf_error(ERR_UNSET_NOT_FOUND);
                status = ERROR;
            }
        }
        i++;
    }
    return status;
}
