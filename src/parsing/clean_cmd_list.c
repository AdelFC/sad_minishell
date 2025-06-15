/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_cmd_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 15:51:27 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/15 15:51:47 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	clean_leading_empty_argv(t_command *cmd)
{
    int	i, j;

    while (cmd)
    {
        i = 0;
        while (cmd->argv && cmd->argv[i] && cmd->argv[i][0] == 0)
            i++;
        if (i > 0 && cmd->argv)
        {
            j = 0;
            while (cmd->argv[i])
            {
                cmd->argv[j] = cmd->argv[i];
                i++;
                j++;
            }
            cmd->argv[j] = NULL;
        }
        cmd = cmd->next;
    }
}