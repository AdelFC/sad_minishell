/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:41:06 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 11:45:31 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_pipe_start_end(t_token *tokens)
{
	t_token	*last;

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

int	error_double_pipe(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
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

static int	check_parenthesis_balance(t_token *tokens, int *open)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->value && ft_strncmp(cur->value, "(", 2) == 0)
			(*open)++;
		else if (cur->value && ft_strncmp(cur->value, ")", 2) == 0)
		{
			if (*open == 0)
				return (ERROR);
			(*open)--;
		}
		cur = cur->next;
	}
	return (SUCCESS);
}

int	error_parenthesis(t_token *tokens)
{
	int	open;

	open = 0;
	if (check_parenthesis_balance(tokens, &open) == ERROR)
	{
		ft_printf_error(ERR_TPC);
		return (ERROR);
	}
	if (open > 0)
	{
		ft_printf_error(ERR_TPO);
		return (ERROR);
	}
	return (SUCCESS);
}
