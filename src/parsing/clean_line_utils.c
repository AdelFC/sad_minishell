/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_line_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:59:06 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 18:00:30 by afodil-c         ###   ########.fr       */
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
