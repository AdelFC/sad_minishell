/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:34:12 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 10:58:48 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_tokenization_and_syntax(char *clean, t_token **tokens)
{
	if (tokenize(clean, tokens) == ERROR)
		return (ERROR);
	if (check_syntax(*tokens) == ERROR)
		return (free_tokens(*tokens), ERROR);
	return (SUCCESS);
}

int	parse_line(char *line, t_shell *sh)
{
	char		*clean;
	t_token		*tokens;
	t_command	*cmd_list;

	sh->tokens = NULL;
	sh->cmds = NULL;
	tokens = NULL;
	if (valid_quotes(line) == ERROR)
		return (ft_printf_error(ERR_UNCLOSED_QUOTE), ERROR);
	clean = clean_line(line);
	if (!clean || !*clean)
		return (free(clean), ERROR);
	if (handle_tokenization_and_syntax(clean, &tokens) == ERROR)
		return (free(clean), ERROR);
	expand_tokens(&tokens, sh->env, sh->last_status);
	sh->tokens = tokens;
	cmd_list = build_commands_from_tokens(tokens);
	if (!cmd_list)
		return (free_tokens(tokens), free(clean), ERROR);
	sh->cmds = cmd_list;
	free(clean);
	return (SUCCESS);
}
