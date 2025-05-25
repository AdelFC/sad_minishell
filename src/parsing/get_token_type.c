/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:57:16 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/25 22:05:54 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	- Determines the type of an operator token */

static int	determine_operator_type(char *value)
{
	if (ft_strncmp(value, "|", 2) == 0)
		return (T_PIPE);
	if (ft_strncmp(value, ">>", 3) == 0)
		return (T_APPEND_OPERATOR);
	if (ft_strncmp(value, "<<", 3) == 0)
		return (T_HEREDOC_OPERATOR);
	if (ft_strncmp(value, ">", 2) == 0)
		return (T_OUTFILE_OPERATOR);
	if (ft_strncmp(value, "<", 2) == 0)
		return (T_INFILE_OPERATOR);
	return (ERROR);
}

/*	- Determines the type of token after an operator */

static int	determine_post_operator_type(t_token *last)
{
	if (last->type == T_INFILE_OPERATOR)
		return (T_INFILE_FILE);
	if (last->type == T_OUTFILE_OPERATOR)
		return (T_OUTFILE_FILE);
	if (last->type == T_APPEND_OPERATOR)
		return (T_APPEND_FILE);
	if (last->type == T_HEREDOC_OPERATOR)
		return (T_HEREDOC_DELIM);
	if (last->type == T_PIPE)
		return (T_CMD);
	return (T_ARG);
}

/*	- Determines the type of a token based on context */

int	get_token_type(char *value, t_token **head)
{
	t_token	*last;
	int		operator_type;

	operator_type = determine_operator_type(value);
	if (operator_type != ERROR)
		return (operator_type);
	if (!*head)
		return (T_CMD);
	last = *head;
	while (last->next)
		last = last->next;
	return (determine_post_operator_type(last));
}
