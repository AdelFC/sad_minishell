/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:09:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/02 11:18:27 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_c_heredoc(int sig)
{
	(void)sig;
	close(STDIN_FILENO);
	g_sig = 130;
}

void	handle_heredoc_input(t_shell *sh, const char *limiter, int fd)
{
	char	*prompt;

	signal(SIGINT, ctrl_c_heredoc);
	rl_clear_history();
	prompt = readline("> ");
	if (g_sig == 130)
		return (sh->last_status = 130, free(prompt));
	while (prompt != NULL)
	{
		if (ft_strncmp(prompt, limiter, ft_strlen(limiter) + 1) == 0)
			break ;
		write(fd, prompt, ft_strlen(prompt));
		write(fd, "\n", 1);
		free(prompt);
		prompt = readline("> ");
		if (g_sig == 130)
			return (sh->last_status = 130, free(prompt));
	}
	free(prompt);
}

static void	child_heredoc_process(const char *limiter, t_shell *sh)
{
	int	fd;

	fd = open(".heredoc.tmp", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
		exit(1);
	handle_heredoc_input(sh, limiter, fd);
	close(fd);
	free_shell(sh);
	if (g_sig == 130)
		exit(130);
	exit(0);
}

static int	parent_heredoc_process(int status)
{
	int	fd;

	if (WIFSIGNALED(status))
	{
		g_sig = 128 + WTERMSIG(status);
		return (128 + WTERMSIG(status));
	}
	if (WEXITSTATUS(status) == 130)
	{
		g_sig = 130;
		return (130);
	}
	if (WEXITSTATUS(status) != 0)
		return (WEXITSTATUS(status));
	fd = open(".heredoc.tmp", O_RDONLY, 0644);
	if (fd < 0)
		return (1);
	return (fd);
}

int	handle_heredoc(const char *limiter, t_shell *sh)
{
	pid_t	pid;
	int		status;
	int		ret;

	g_sig = 0;
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		child_heredoc_process(limiter, sh);
	waitpid(pid, &status, 0);
	ret = parent_heredoc_process(status);
	if (ret >= 128 || ret == 1 || ret == 130)
		sh->last_status = ret;
	return (ret);
}
