/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:36:46 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/02 11:18:42 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_sig = 0;

static int	prepare_heredocs(t_command *cmds, t_shell *sh)
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
				fd = handle_heredoc(redir->filename, sh);
				if (fd < 0)
					return (ERROR);
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

static t_shell	*init_shell_and_signals(char **envp)
{
	t_shell	*sh;

	sh = init_shell(envp);
	if (!sh)
	{
		ft_printf_error(ERR_NO_ENV);
		return (NULL);
	}
	init_signals();
	return (sh);
}

static int	handle_heredoc_and_exec(t_shell *sh)
{
	if (prepare_heredocs(sh->cmds, sh) == ERROR)
	{
		if (g_sig == 130)
		{
			sh->last_status = 130;
			free_commands(sh->cmds);
			free_tokens(sh->tokens);
			sh->cmds = NULL;
			sh->tokens = NULL;
			return (SUCCESS);
		}
		sh->last_status = 1;
		return (ERROR);
	}
	exec_commands(sh);
	cleanup_heredocs_list(sh->cmds);
	free_commands(sh->cmds);
	free_tokens(sh->tokens);
	sh->cmds = NULL;
	sh->tokens = NULL;
	return (SUCCESS);
}

static int	process_line(char *line, t_shell *sh)
{
	int	ret;

	if (!line)
	{
		printf("exit\n");
		return (ERROR);
	}
	ret = parse_line(line, sh);
	if (ret != SUCCESS)
	{
		sh->last_status = 2;
		free(line);
		return (SUCCESS);
	}
	handle_heredoc_and_exec(sh);
	free(line);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;
	char	*line;
	int		last_status;

	if (init_basics(argc, argv) == ERROR)
		return (ERROR);
	sh = init_shell_and_signals(envp);
	if (!sh)
		return (ERROR);
	while (1)
	{
		line = init_prompt();
		if (g_sig)
		{
			sh->last_status = g_sig;
			g_sig = 0;
		}
		if (!line || process_line(line, sh) == ERROR)
			break ;
	}
	last_status = sh->last_status;
	free_shell(sh);
	return (last_status);
}

/*static const char	*type_to_str(int type)
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

static void	debug_print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("[%s:\"%s\"] ", type_to_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("\n");
}*/