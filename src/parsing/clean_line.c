/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:11:27 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/12 15:12:57 by afodil-c         ###   ########.fr       */
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
		char_buffer(var->res, &var->j, ' ');
		var->space = 0;
	}
	char_buffer(var->res, &var->j, var->line[var->i]);
	var->i++;
}

static void	process_operator(t_clean_line *var)
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

static void	clean_line_loop(t_clean_line *var)
{
	char	c;

	while (var->line[var->i])
	{
		c = var->line[var->i];
		if ((c == '\'') && (!var->in_dquote))
		{
			if (var->space && var->j > 0)
			{
				char_buffer(var->res, &var->j, ' ');
				var->space = 0;
			}
			var->in_squote = !var->in_squote;
			char_buffer(var->res, &var->j, c);
			var->i++;
			continue ;
		}
		if (c == '"' && !var->in_squote)
		{
			if (var->space && var->j > 0)
			{
				char_buffer(var->res, &var->j, ' ');
				var->space = 0;
			}
			var->in_dquote = !var->in_dquote;
			char_buffer(var->res, &var->j, c);
			var->i++;
			continue ;
		}
		if (var->in_squote || var->in_dquote)
		{
			char_buffer(var->res, &var->j, c);
			var->i++;
		}
		else if ((unsigned char)c < 32 && c != '\t')
			var->i++;
		else if (is_operator(c) == SUCCESS)
			process_operator(var);
		else if (is_space(c) == SUCCESS)
		{
			var->space = 1;
			var->i++;
		}
		else
			process_regular_char(var);
	}
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
	clean_line_loop(&var);
	var.res[var.j] = '\0';
	return (var.res);
}
