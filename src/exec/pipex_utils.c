/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:25:46 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/10 14:00:04 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_prev_fd(int *prev_fd, t_command *cur)
{
	if (cur->next)
	{
		close(cur->fd[1]);
		if (*prev_fd != -1)
			close(*prev_fd);
		*prev_fd = cur->fd[0];
	}
}

void	handle_pipe_iteration(t_command *cur, t_shell *sh, int prev_fd)
{
	pid_t	pid;

	if (cur->next && pipe(cur->fd) < 0)
		ft_printf_error(ERR_PIPE);
	pid = fork();
	if (pid < 0)
		ft_printf_error(ERR_FORK);
	else if (pid == 0)
	{
		signal(SIGPIPE, sigpipe_handler);
		b_sig();
		if (!cur->next)
			process_last(prev_fd, cur, sh);
		else if (prev_fd == -1)
			process_first(cur, sh);
		else
			process_middle(prev_fd, cur, sh);
	}
}
