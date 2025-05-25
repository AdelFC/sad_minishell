/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 00:40:13 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 00:47:09 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_pipe(t_command **current, t_command **head)
{
    t_command *next = new_command();

    if (!next)
        return ERROR;
    (*current)->next = next;
    *current = next;
    return SUCCESS;
}

/*
** Transforme tokens en liste de commandes
*/
t_command *build_commands_from_tokens(t_token *tokens)
{
    t_command *head    = NULL;
    t_command *current = NULL;

    while (tokens)
    {
        if (!current)
        {
            current = new_command();
            if (!current)
                return (NULL);
            head = current;
        }
        if (tokens->type == T_CMD || tokens->type == T_ARG)
        {
            if (add_argv(current, tokens->value) == ERROR)
                return (NULL);
        }
        else if (tokens->type == T_INFILE_OPERATOR)
        {
            if (add_redir(current, T_INFILE_OPERATOR, tokens->next->value) == ERROR)
                return (NULL);
            tokens = tokens->next;
        }
        else if (tokens->type == T_OUTFILE_OPERATOR)
        {
            if (add_redir(current, T_OUTFILE_OPERATOR, tokens->next->value) == ERROR)
                return (NULL);
            tokens = tokens->next;
        }
        else if (tokens->type == T_APPEND_OPERATOR)
        {
            if (add_redir(current, T_APPEND_OPERATOR, tokens->next->value) == ERROR)
                return (NULL);
            tokens = tokens->next;
        }
        else if (tokens->type == T_HEREDOC_OPERATOR)
        {
            if (add_redir(current, T_HEREDOC_OPERATOR, tokens->next->value) == ERROR)
                return (NULL);
            tokens = tokens->next;
        }
        else if (tokens->type == T_PIPE)
        {
            if (handle_pipe(&current, &head) == ERROR)
                return (NULL);
        }
        tokens = tokens->next;
    }
    return (head);
}
