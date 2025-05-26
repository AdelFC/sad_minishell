/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:09 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:33:25 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** pwd: affiche le répertoire courant.
** Erreur si des arguments sont passés.
*/
int ft_pwd(char **argv)
{
    char *cwd;

    (void)argv;
    if (argv[1])
    {
        ft_printf_error(ERR_PWD_TOO_MANY_ARGS);
        return (ERROR);
    }
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        ft_printf_error(ERR_PWD_CANNOT_CWD);
        return (ERROR);
    }
    printf("%s\n", cwd);
    free(cwd);
    return (SUCCESS);
}
