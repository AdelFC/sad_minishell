/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:20:45 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 00:23:41 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_infile(const char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_printf_error("minishell: infile: filename is NULL\n");
		return (ERROR);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf_error(ERR_MINISHELL_PERMISSION, filename);
		return (ERROR);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		ft_printf_error("minishell: dup2: %s\n", strerror(errno));
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

static int	handle_outfile(const char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_printf_error("minishell: outfile: filename is NULL\n");
		return (ERROR);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf_error(ERR_MINISHELL_PERMISSION, filename);
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		ft_printf_error("minishell: dup2: %s\n", strerror(errno));
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

static int	handle_append(const char *filename)
{
	int	fd;

	if (!filename)
	{
		ft_printf_error("minishell: append: filename is NULL\n");
		return (ERROR);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_printf_error(ERR_MINISHELL_PERMISSION, filename);
		return (ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		ft_printf_error("minishell: dup2: %s\n", strerror(errno));
		return (ERROR);
	}
	close(fd);
	return (SUCCESS);
}

static int	apply_single_redir(t_redir *redir)
{
	int	status;

	if (redir->type == T_INFILE_FILE)
		status = handle_infile(redir->filename);
	else if (redir->type == T_OUTFILE_FILE)
		status = handle_outfile(redir->filename);
	else if (redir->type == T_APPEND_FILE)
		status = handle_append(redir->filename);
	else if (redir->type == T_HEREDOC_DELIM)
	{
		if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
		{
			close(redir->heredoc_fd);
			ft_printf_error("minishell: dup2 heredoc: %s\n",
				strerror(errno));
			return (ERROR);
		}
		close(redir->heredoc_fd);
		status = SUCCESS;
	}
	else
		status = SUCCESS;
	return (status);
}

int	apply_redirections(t_redir *redir)
{
	int	status;

	status = 0;
	while (redir)
	{
		status = apply_single_redir(redir);
		if (status == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (SUCCESS);
}
