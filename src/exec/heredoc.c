/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:09:51 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 17:21:05 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Lis l'entrée utilisateur ligne à ligne jusqu'au 'limiter'.
** Écrit chaque ligne dans fd, sauf la ligne d'arrêt.
*/
void	handle_heredoc_input(const char *limiter, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
        if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

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
	handle_heredoc_input(limiter, h.fd_write);
	close(h.fd_write);
	h.fd_read = open(h.filename, O_RDONLY);
	unlink(h.filename);
	free(h.pid_str);
	free(h.cnt_str);
	return (h.fd_read);
}
