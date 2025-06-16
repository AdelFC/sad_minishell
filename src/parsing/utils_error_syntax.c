/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:46:24 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 09:50:27 by afodil-c         ###   ########.fr       */
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
	while (cur)
	{
		if (is_redir_op(cur->type))
		{
			if (!cur->next)
			{
				ft_printf_error
				("syntax error near unexpected token `newline`\n");
				return (ERROR);
			}
			if (!is_redir_file(cur->next->type) && !(cur->next->type == T_CMD
					|| cur->next->type == T_ARG))
			{
				ft_printf_error("syntax error near unexpected token `%s`\n",
					cur->next->value);
				return (ERROR);
			}
		}
		cur = cur->next;
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
