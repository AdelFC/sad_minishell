/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:11:54 by barnaud           #+#    #+#             */
/*   Updated: 2025/05/29 12:19:44 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirections_and_save(t_command *cmd, t_shell *sh, int *in_save,
		int *out_save)
{
	*in_save = dup(STDIN_FILENO);
	*out_save = dup(STDOUT_FILENO);
	if (cmd->redirs && apply_redirections(cmd->redirs) == ERROR)
	{
		restore_std_fds(*in_save, *out_save);
		return (sh->last_status);
	}
	return (0);
}

int	handle_exit_builtin(t_command *cmd, t_shell *sh, int in_save, int out_save)
{
	if (!ft_strncmp(cmd->argv[0], "exit", 5) && cmd->argv[0][4] == '\0')
	{
		restore_std_fds(in_save, out_save);
		ft_exit(cmd->argv, sh);
		return (1);
	}
	return (0);
}

void	handle_redir_without_cmd(t_command *cmd)
{
	int	in_save;
	int	out_save;

	in_save = dup(STDIN_FILENO);
	out_save = dup(STDOUT_FILENO);
	if (cmd->redirs)
		apply_redirections(cmd->redirs);
	dup2(in_save, STDIN_FILENO);
	dup2(out_save, STDOUT_FILENO);
	close(in_save);
	close(out_save);
	cleanup_heredocs_list(cmd);
}
