/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:16 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:28:26 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Returns SUCCESS (0) if s is a valid integer string ("±?[0-9]+"), ERROR (-1) otherwise.
*/
static int is_numeric(const char *s)
{
    int i = 0;

    if (!s || s[0] == '\0')
        return (ERROR);
    if (s[i] == '+' || s[i] == '-')
        i++;
    if (s[i] == '\0')
        return (ERROR);
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return (ERROR);
        i++;
    }
    return (SUCCESS);
}

/*
** Builtin exit :
** - Aucun argument        → exit(EXIT_SUCCESS)
** - Un argument numérique → exit(atoi(arg))
** - Un argument non-num   → print ERR_EXIT_NUM_ARG, exit(2)
** - Plusieurs arguments  → print ERR_EXIT_TOO_MANY_ARGS, return ERROR
**
** Affiche "exit\n" avant de quitter (sauf sur erreur nombre d’args).
*/
int ft_exit(char **argv)
{
    long code = 0;
    int  argc = 0;
    int  i = 1;

    while (argv[i])
    {
        argc++;
        i++;
    }
    if (argc > 1)
    {
        ft_printf_error(ERR_EXIT_TOO_MANY_ARGS);
        return (ERROR);
    }
    if (argc == 1)
    {
        if (is_numeric(argv[1]) == ERROR)
        {
            ft_printf_error(ERR_EXIT_NUM_ARG, argv[1]);
            exit(2);
        }
        code = ft_atoi(argv[1]);
    }
    ft_printf("exit\n");
    exit((int)code);
}
