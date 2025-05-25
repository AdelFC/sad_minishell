/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:41:06 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 00:01:03 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Vérifie qu’il n’y a pas de pipe en début ou fin de liste
*/
int error_pipe_start_end(t_token *tokens)
{
    t_token *last;

    if (!tokens)
        return (SUCCESS);
    if (is_pipe(tokens->type))
    {
        ft_printf_error(ERR_TOKEN_PIPE);
        return (ERROR);
    }
    last = tokens;
    while (last->next)
        last = last->next;
    if (is_pipe(last->type))
    {
        ft_printf_error(ERR_TOKEN_PIPE);
        return (ERROR);
    }
    return (SUCCESS);
}

/*
** Vérifie qu’il n’y a pas deux pipes consécutifs
*/
int error_double_pipe(t_token *tokens)
{
    t_token *cur = tokens;

    while (cur && cur->next)
    {
        if (is_pipe(cur->type) && is_pipe(cur->next->type))
        {
            ft_printf_error(ERR_TOKEN_PIPE);
            return (ERROR);
        }
        cur = cur->next;
    }
    return (SUCCESS);
}

/*
** Vérifie la validité des redirections :
**   - après un opérateur, on attend un fichier
**   - on ne peut pas terminer par un opérateur
*/
int error_redirection(t_token *tokens)
{
    t_token *cur = tokens;

    while (cur && cur->next)
    {
        if (is_redir_op(cur->type) && !is_redir_file(cur->next->type))
        {
            ft_printf_error(ERR_UNEXPECTED_TOKEN, cur->next->value);
            return (ERROR);
        }
        cur = cur->next;
    }
    if (cur && is_redir_op(cur->type))
    {
        ft_printf_error(ERR_TOKEN_NEWLINE);
        return (ERROR);
    }
    return (SUCCESS);
}

/*
** Vérifie qu’il n’y a pas de ";;" dans la liste des tokens
*/
int error_double_semicolon(t_token *tokens)
{
    t_token *cur = tokens;

    while (cur && cur->next)
    {
        if (cur->value
         && ft_strncmp(cur->value, ";;", 3) == 0)
        {
            ft_printf_error(ERR_TOKEN_DOUBLE_SEMICOLON);
            return (ERROR);
        }
        cur = cur->next;
    }
    return (SUCCESS);
}

/*
** Vérifie la bonne parenthésisation (si tu gères les () dans ton shell)
*/
int error_parenthesis(t_token *tokens)
{
    t_token *cur = tokens;
    int      depth = 0;

    while (cur)
    {
        if (cur->value && ft_strncmp(cur->value, "(", 2) == 0)
            depth++;
        else if (cur->value && ft_strncmp(cur->value, ")", 2) == 0)
        {
            if (depth == 0)
            {
                ft_printf_error(ERR_TOKEN_PAREN_CLOSE);
                return (ERROR);
            }
            depth--;
        }
        cur = cur->next;
    }
    if (depth > 0)
    {
        ft_printf_error(ERR_TOKEN_PAREN_OPEN);
        return (ERROR);
    }
    return (SUCCESS);
}
