/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:09:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 22:15:09 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void heredoc_sigint_handler(int sig)
{
	(void)sig;
	exit(130);
}

void	handle_heredoc_input(const char *limiter, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(const char *limiter, t_shell *sh)
{
	t_heredoc_utils	h;
	static int		count = 0;
	pid_t			pid;
	int				status = 0;

	h.pid_str = ft_itoa(getpid());
	h.cnt_str = ft_itoa(count++);
	ft_strlcpy(h.filename, "/tmp/minishell_heredoc_", sizeof(h.filename));
	ft_strlcat(h.filename, h.pid_str, sizeof(h.filename));
	ft_strlcat(h.filename, "_", sizeof(h.filename));
	ft_strlcat(h.filename, h.cnt_str, sizeof(h.filename));
	h.fd_write = -1;
	h.fd_read = -1;

	pid = fork();
	if (pid < 0)
	{
		ft_printf_error(ERR_FORK);
		free(h.pid_str);
		free(h.cnt_str);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		h.fd_write = open(h.filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (h.fd_write < 0)
		{
			free(h.pid_str);
			free(h.cnt_str);
			free_shell(sh);
			exit(1);
		}
		handle_heredoc_input(limiter, h.fd_write);
		close(h.fd_write);
		free(h.pid_str);
		free(h.cnt_str);
		free_shell(sh);
		exit(0);
	}
	waitpid(pid, &status, 0);
	// Toujours tenter d'ouvrir le fichier pour pouvoir le fermer si besoin
	h.fd_read = open(h.filename, O_RDONLY);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) ||
		(WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		g_sig = 130;
		if (h.fd_read >= 0)
		{
			close(h.fd_read);
			unlink(h.filename);
		}
		else
			unlink(h.filename);
		free(h.pid_str);
		free(h.cnt_str);
		// free_shell(sh); // NE PAS free ici, le shell est libéré dans le main
		return (-1);
	}
	if (h.fd_read >= 0)
		unlink(h.filename);
	free(h.pid_str);
	free(h.cnt_str);
	return (h.fd_read);
}
