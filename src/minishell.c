/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:36:46 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 22:08:18 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*type_to_str(int type)
{
	if (type == T_CMD)
		return ("Commande");
	else if (type == T_ARG)
		return ("Argument");
	else if (type == T_PIPE)
		return ("Pipe");
	else if (type == T_INFILE_FILE)
		return ("Infile");
	else if (type == T_INFILE_OPERATOR)
		return ("Infile operator");
	else if (type == T_OUTFILE_FILE)
		return ("Outfile");
	else if (type == T_OUTFILE_OPERATOR)
		return ("Outfile operator");
	else if (type == T_APPEND_FILE)
		return ("Append");
	else if (type == T_APPEND_OPERATOR)
		return ("Append operator");
	else if (type == T_HEREDOC_DELIM)
		return ("Heredoc");
	else if (type == T_HEREDOC_OPERATOR)
		return ("Heredoc operator");
	return ("UNKNOWN");
}

void	debug_print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("[%s:\"%s\"] ", type_to_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("\n");
}

static int	prepare_heredocs(t_command *cmds)
{
	t_command	*cmd;
	t_redir		*redir;
	int			fd;

	cmd = cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC_DELIM)
			{
				fd = handle_heredoc(redir->filename);
				if (fd < 0)
					return (ft_printf_error("minishell: heredoc failed\n"),
						ERROR);
				free(redir->filename);
				redir->filename = NULL;
				redir->heredoc_fd = fd;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;
	char	*line;
	int		last_status;

	(void)argc;
	(void)argv;
	sh = init_shell(envp);
	if (!sh)
		return (ft_printf_error(ERR_NO_ENV), ERROR);
	init_signals();
	while (1)
	{
		line = init_prompt();
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (parse_line(line, sh) == SUCCESS)
		{
			if (prepare_heredocs(sh->cmds) == ERROR)
			{
				sh->last_status = 1;
				free(line);
				break ;
			}
			exec_commands(sh);
			free_commands(sh->cmds);
			free_tokens(sh->tokens);
			sh->cmds = NULL;
			sh->tokens = NULL;
		}
		free(line);
	}
	last_status = sh->last_status;
	free_shell(sh);
	return (last_status);
}
