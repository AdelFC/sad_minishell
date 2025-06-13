/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:43:16 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 15:43:19 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe(t_command **current)
{
	t_command	*next;

	next = new_command();
	if (!next)
		return (ERROR);
	(*current)->next = next;
	*current = next;
	return (SUCCESS);
}

int	handle_simple_token(t_command **cur, t_token *tok)
{
	if (tok->type == T_CMD || tok->type == T_ARG)
		return (add_argv(*cur, tok->value));
	if (tok->type == T_PIPE)
		return (handle_pipe(cur));
	return (ERROR);
}
