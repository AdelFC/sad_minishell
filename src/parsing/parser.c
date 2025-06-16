/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:04:05 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 09:53:07 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes_and_clean(char *line, t_shell *sh, char **clean)
{
	if (valid_quotes(line) == ERROR)
	{
		sh->last_status = 1;
		ft_printf_error(ERR_UNCLOSED_QUOTE);
		return (ERROR);
	}
	*clean = clean_line(line);
	if (!*clean || !**clean)
	{
		sh->last_status = 1;
		free(*clean);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	handle_tokenize_and_syntax(char *clean, t_shell *sh,
		t_token **tokens)
{
	if (tokenize(clean, tokens) == ERROR)
	{
		sh->last_status = 1;
		free(clean);
		return (ERROR);
	}
	if (check_syntax(*tokens) == ERROR)
	{
		sh->last_status = 2;
		free_tokens(*tokens);
		free(clean);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	handle_split_and_build(char *clean, t_shell *sh, t_token *tokens,
		t_command **cmd_list)
{
	t_token	**cmd_splits;

	cmd_splits = split_commands(tokens);
	if (!cmd_splits)
	{
		sh->last_status = 1;
		free_tokens(tokens);
		free(clean);
		return (ERROR);
	}
	*cmd_list = build_commands_from_splits(cmd_splits);
	if (!*cmd_list)
	{
		sh->last_status = 1;
		free_cmd_splits(cmd_splits);
		free_tokens(tokens);
		free(clean);
		return (ERROR);
	}
	free_cmd_splits(cmd_splits);
	return (SUCCESS);
}

int	parse_line(char *line, t_shell *sh)
{
	char		*clean;
	t_token		*tokens;
	t_command	*cmd_list;

	clean = NULL;
	tokens = NULL;
	cmd_list = NULL;
	sh->tokens = NULL;
	sh->cmds = NULL;
	if (handle_quotes_and_clean(line, sh, &clean) == ERROR)
		return (ERROR);
	if (handle_tokenize_and_syntax(clean, sh, &tokens) == ERROR)
		return (ERROR);
	expand_tokens(&tokens, sh->env, sh->last_status);
	sh->tokens = tokens;
	if (handle_split_and_build(clean, sh, tokens, &cmd_list) == ERROR)
		return (ERROR);
	sh->cmds = cmd_list;
	clean_leading_empty_argv(sh->cmds);
	sh->tokens = NULL;
	free(clean);
	return (SUCCESS);
}
