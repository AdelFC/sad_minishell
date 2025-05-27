/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:44:12 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 09:55:16 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	char_buffer(char *res, int *j, char c)
{
	res[*j] = c;
	(*j)++;
}

static void	process_regular_char(t_clean_line *var)
{
	if (var->space && var->j > 0)
	{
		char_buffer(var->res, &var->j, 32);
		var->space = 0;
	}
	char_buffer(var->res, &var->j, var->line[var->i]);
	var->i++;
}

static void	process_operator(t_clean_line *var)
{
	if (var->j > 0 && var->res[var->j - 1] != 32)
		char_buffer(var->res, &var->j, 32);
	char_buffer(var->res, &var->j, var->line[var->i]);
	if ((var->line[var->i] == '<' || var->line[var->i] == '>')
		&& var->line[var->i + 1] == var->line[var->i])
	{
		char_buffer(var->res, &var->j, var->line[var->i + 1]);
		var->i++;
	}
	if (is_space(var->line[var->i + 1]) == ERROR)
		char_buffer(var->res, &var->j, 32);
	var->i++;
}

static void	init_clean_line_vars(t_clean_line *var, char *line)
{
	var->line = line;
	var->i = skip_spaces(line);
	var->j = 0;
	var->space = 0;
	var->in_squote = 0;
	var->in_dquote = 0;
}

char	*clean_line(char *line)
{
	t_clean_line	var;

	if (!line)
		return (NULL);
	var.res = malloc(ft_strlen(line) * 2 + 1);
	if (!var.res)
		return (NULL);
	init_clean_line_vars(&var, line);
	while (var.line[var.i])
	{
		if (var.line[var.i] == '\'' && !var.in_dquote)
			var.in_squote = !var.in_squote;
		else if (var.line[var.i] == '"' && !var.in_squote)
			var.in_dquote = !var.in_dquote;
		if ((unsigned char)var.line[var.i] < 32 && var.line[var.i] != '\t')
			var.i++;
		else if (is_operator(var.line[var.i]) == SUCCESS)
			process_operator(&var);
		else if (is_space(var.line[var.i]) == SUCCESS && !var.in_squote
			&& !var.in_dquote)
		{
			var.space = 1;
			var.i++;
		}
		else
			process_regular_char(&var);
	}
	var.res[var.j] = '\0';
	return (var.res);
}
