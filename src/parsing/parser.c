/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:04:05 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 13:24:39 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(char *line, t_shell *sh)
{
	char		*clean = NULL;
	t_token		*tokens = NULL;
	t_command	*cmd_list = NULL;
	t_token		**cmd_splits = NULL;

	sh->tokens = NULL;
	sh->cmds = NULL;

	// Vérification des quotes et nettoyage de la ligne
	if (valid_quotes(line) == ERROR)
	{
		sh->last_status = 2;
		ft_printf_error(ERR_UNCLOSED_QUOTE);
		return (ERROR);
	}
	clean = clean_line(line);
	if (!clean || !*clean)
	{
		sh->last_status = 2;
		free(clean);
		return (ERROR);
	}

	// Tokenisation
	if (tokenize(clean, &tokens) == ERROR)
	{
		sh->last_status = 2;
		free(clean);
		return (ERROR);
	}

	// Vérification de la syntaxe
	if (check_syntax(tokens) == ERROR)
	{
		sh->last_status = 2;
		free_tokens(tokens);
		free(clean);
		return (ERROR);
	}

	// Expansion
	expand_tokens(&tokens, sh->env, sh->last_status);
	sh->tokens = tokens;

	// Split des commandes
	cmd_splits = split_commands(tokens);
	if (!cmd_splits)
	{
		sh->last_status = 2;
		free_tokens(tokens);
		free(clean);
		return (ERROR);
	}

	// Construction de la liste de commandes
	cmd_list = build_commands_from_splits(cmd_splits);
	if (!cmd_list)
	{
		sh->last_status = 2;
		free_cmd_splits(cmd_splits);
		free_tokens(tokens);
		free(clean);
		return (ERROR);
	}

	sh->cmds = cmd_list;
	free_cmd_splits(cmd_splits);
	sh->tokens = NULL; // Empêche le double free dans free_shell
	free(clean);
	return (SUCCESS);
}

/*
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

static void	debug_print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("[%s:\"%s\"] ", type_to_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("\n");
}

int	parse_line(char *line, t_shell *sh)
{
	char		*clean;
	t_token		*tokens;
	t_command	*cmd_list;
	t_token  **cmd_splits;

	printf("[QUOTE CHECK] Début du parsing de la ligne : \"%s\"\n", line);
	sh->tokens = NULL;
	sh->cmds = NULL;
	tokens = NULL;
	if (valid_quotes(line) == ERROR)
	{
		printf("[QUOTE CHECK][ERREUR] Quotes invalides \
		détectées dans : \"%s\"\n", line);
		return (ft_printf_error(ERR_UNCLOSED_QUOTE), ERROR);
	}
	printf("[QUOTE CHECK] Quotes valides\n");
	printf("[CLEAN LINE] Nettoyage de la ligne...\n");
	clean = clean_line(line);
	if (!clean || !*clean)
	{
		printf("[CLEAN LINE][ERREUR] Ligne vide ou \
		 nettoyage échoué : \"%s\"\n", line);
		return (free(clean), ERROR);
	}
	printf("[CLEAN LINE] Ligne nettoyée : \"%s\"\n", clean);
	printf("[TOKENIZE] Début de la tokenisation...\n");
	if (tokenize(clean, &tokens) == ERROR)
	{
		printf("[TOKENIZE][ERREUR] Tokenisation échouée sur : \"%s\"\n", clean);
		return (free(clean), ERROR);
	}
	printf("[TOKENIZE] Tokenisation réussie\n");
	printf("[TOKENIZE] Tokens obtenus (avant expansion) : \n");
	debug_print_tokens(tokens);

	printf("[SYNTAX CHECK] Vérification de la syntaxe...\n");
	if (check_syntax(tokens) == ERROR)
	{
		printf("[SYNTAX CHECK][ERREUR] Syntaxe invalide sur tokens\n");
		return (free_tokens(tokens), free(clean), ERROR);
	}
	printf("[SYNTAX CHECK] Syntaxe valide\n");

	printf("[EXPAND] Expansion des tokens...\n");
	expand_tokens(&tokens, sh->env, sh->last_status);
	printf("[TOKENIZE] Tokens obtenus (après expansion) : \n");
	debug_print_tokens(tokens);

	sh->tokens = tokens;
	cmd_splits = split_commands(tokens);
	if (!cmd_splits)
	{
		printf("[CMD_SPLIT][ERREUR] Découpage des commandes échoué\n");
		return (free_tokens(tokens), free(clean), ERROR);
	}
	printf("[CMD_SPLIT] Découpage des tokens en commandes :\n");
	for (int i = 0; cmd_splits[i] != NULL; i++)
	{
		printf("  Commande %d : ", i);
		debug_print_tokens(cmd_splits[i]);
	}

	printf("[BUILD CMDS] Construction de la liste de commandes...\n");
	cmd_list = build_commands_from_splits(cmd_splits);
	if (!cmd_list)
	{
		printf("[BUILD CMDS][ERREUR] Construction de la liste de \ 
		commandes échouée sur tokens\n");
		return (free_cmd_splits(cmd_splits), free(clean), ERROR);
	}
	printf("[BUILD CMDS] Liste de commandes construite avec succès\n");
	sh->cmds = cmd_list;
	free(clean);
	printf("[END] Parsing terminé avec succès\n");
	return (SUCCESS);
}
*/