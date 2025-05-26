/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 23:44:12 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 22:16:57 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*	- Append a char to buffer and advance position */

static void	char_buffer(char *res, int *j, char c)
{
	res[*j] = c;
	(*j)++;
}

/*	- Handle normal chars and single spacing */

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

/*	- Handle operators "|", "<", ">" and their doubles "<<", ">>" */

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

/* clean line:
	- Initialisation of var for norm */

static void	init_clean_line_vars(t_clean_line *var, char *line)
{
	var->line = line;
	var->i = skip_spaces(line);
	var->j = 0;
	var->space = 0;
    var->in_squote = 0;
    var->in_dquote = 0;
}

/*	- Skip leading spaces/tabs
	- Filter ASCII<32 (except tab)
	- Normalize spaces
	- Add spaces around operators */

char *clean_line(char *line)
{
    t_clean_line var;

    if (!line)
        return NULL;
    var.res = malloc(ft_strlen(line) * 2 + 1);
    if (!var.res)
        return NULL;
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
        else if (is_space(var.line[var.i]) == SUCCESS
                && !var.in_squote && !var.in_dquote)
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
