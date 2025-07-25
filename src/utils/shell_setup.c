/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_setup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:36:57 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/27 22:04:45 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*sh;

	sh = malloc(sizeof(t_shell));
	if (!sh)
		return (NULL);
	sh->env = init_env(envp);
	if (!sh->env)
	{
		free(sh);
		return (NULL);
	}
	sh->envp = envp;
	sh->tokens = NULL;
	sh->cmds = NULL;
	sh->last_status = 0;
	return (sh);
}

void	free_shell(t_shell *sh)
{
	if (!sh)
		return ;
	free_commands(sh->cmds);
	free_tokens(sh->tokens);
	free_env(sh->env);
	rl_clear_history();
	free(sh);
}
