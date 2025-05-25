/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:46:24 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/25 22:47:16 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helpers pour clarifier les tests de type */
int is_pipe(int type)
{
    return (type == T_PIPE);
}

int is_redir_op(int type)
{
    return (type == T_INFILE_OPERATOR
         || type == T_OUTFILE_OPERATOR
         || type == T_APPEND_OPERATOR
         || type == T_HEREDOC_OPERATOR);
}

int is_redir_file(int type)
{
    return (type == T_INFILE_FILE
         || type == T_OUTFILE_FILE
         || type == T_APPEND_FILE
         || type == T_HEREDOC_DELIM);
}
