/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line_U.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:38:02 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 15:39:00 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	char_buffer(char *res, int *j, char c)
{
	res[*j] = c;
	(*j)++;
}

void	process_regular_char(t_clean_line *var)
{
	if (var->space && var->j > 0)
	{
		char_buffer(var->res, &var->j, ' ');
		var->space = 0;
	}
	char_buffer(var->res, &var->j, var->line[var->i]);
	var->i++;
}

void	process_operator(t_clean_line *var)
{
	if (var->space && var->j > 0)
	{
		char_buffer(var->res, &var->j, ' ');
		var->space = 0;
	}
	char_buffer(var->res, &var->j, var->line[var->i]);
	if ((var->line[var->i] == '<' || var->line[var->i] == '>')
		&& var->line[var->i + 1] == var->line[var->i])
	{
		char_buffer(var->res, &var->j, var->line[var->i + 1]);
		var->i++;
	}
	if (var->line[var->i + 1] && !is_space(var->line[var->i + 1]))
		char_buffer(var->res, &var->j, ' ');
	var->i++;
}
