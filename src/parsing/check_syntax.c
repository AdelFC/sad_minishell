/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:12:33 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/15 16:28:00 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(t_token *tokens)
{
	if (error_pipe_start_end(tokens) == ERROR)
		return (ERROR);
	if (error_double_pipe(tokens) == ERROR)
		return (ERROR);
	if (error_redirection(tokens) == ERROR)
		return (ERROR);
	if (error_double_semicolon(tokens) == ERROR)
		return (ERROR);
	if (error_parenthesis(tokens) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
