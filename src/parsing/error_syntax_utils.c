/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:46:24 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 11:45:56 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(int type)
{
	return (type == T_PIPE);
}

int	is_redir_op(int type)
{
	return (type == T_INFILE_OPERATOR || type == T_OUTFILE_OPERATOR
		|| type == T_APPEND_OPERATOR || type == T_HEREDOC_OPERATOR);
}

int	is_redir_file(int type)
{
	return (type == T_INFILE_FILE || type == T_OUTFILE_FILE
		|| type == T_APPEND_FILE || type == T_HEREDOC_DELIM);
}

int	error_redirection(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur && cur->next)
	{
		if (is_redir_op(cur->type) && !is_redir_file(cur->next->type))
		{
			ft_printf_error(ERR_UT, cur->next->value);
			return (ERROR);
		}
		cur = cur->next;
	}
	if (cur && is_redir_op(cur->type))
	{
		ft_printf_error(ERR_TN);
		return (ERROR);
	}
	return (SUCCESS);
}

int	error_double_semicolon(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur && cur->next)
	{
		if (cur->value && ft_strncmp(cur->value, ";;", 3) == 0)
		{
			ft_printf_error(ERR_DS);
			return (ERROR);
		}
		cur = cur->next;
	}
	return (SUCCESS);
}
