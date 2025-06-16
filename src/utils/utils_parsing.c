/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:48:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 09:49:03 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_clean_line_vars(t_clean_line *var, char *line)
{
	var->line = line;
	var->i = skip_spaces(line);
	var->j = 0;
	var->space = 0;
	var->in_squote = 0;
	var->in_dquote = 0;
}

int	is_space(char c)
{
	if (c == 32 || c == 9)
		return (SUCCESS);
	return (ERROR);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (SUCCESS);
	return (ERROR);
}

int	skip_spaces(char *line)
{
	int	i;

	i = 0;
	while (is_space(line[i]) == SUCCESS)
		i++;
	return (i);
}
