/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:09:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 12:07:30 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// static int	process_heredoc_line(const char *limiter, int fd, char *line)
// {
// 	if (!line)
// 		return (0);
// 	if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
// 	{
// 		free(line);
// 		return (0);
// 	}
// 	write(fd, line, ft_strlen(line));
// 	write(fd, "\n", 1);
// 	free(line);
// 	return (ERROR);
// }

// void	handle_heredoc_input(const char *limiter, int fd)
// {
// 	char	*line;

// 	while (1)
// 	{
// 		while (g_wait != 3)
// 		{
// 			if (g_wait == 3)
// 				return ;
// 			line = readline("> ");
// 			if (process_heredoc_line(limiter, fd, line) == ERROR)
// 				break ;
// 			if (g_wait == 3)
// 			{
// 				if (line)
// 					free(line);
// 				return ;
// 			}
// 		}
// 	}
// }

static void	init_heredoc_utils(t_heredoc_utils *h, int *static_count)
{
	h->pid_str = ft_itoa(getpid());
	h->cnt_str = ft_itoa((*static_count)++);
	ft_strlcpy(h->filename, "/tmp/minishell_heredoc_", sizeof(h->filename));
	ft_strlcat(h->filename, h->pid_str, sizeof(h->filename));
	ft_strlcat(h->filename, "_", sizeof(h->filename));
	ft_strlcat(h->filename, h->cnt_str, sizeof(h->filename));
	h->fd_write = -1;
	h->fd_read = -1;
}

int	handle_heredoc(const char *limiter)
{
	t_heredoc_utils	h;
	static int		count = 0;

	init_heredoc_utils(&h, &count);
	h.fd_write = open(h.filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (h.fd_write < 0)
	{
		free(h.pid_str);
		free(h.cnt_str);
		return (-1);
	}
	heredoc_signal_handler();
	handle_heredoc_input(limiter, h.fd_write);
	close(h.fd_write);
	h.fd_read = open(h.filename, O_RDONLY);
	unlink(h.filename);
	free(h.pid_str);
	free(h.cnt_str);
	return (h.fd_read);
}
