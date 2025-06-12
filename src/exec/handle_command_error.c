/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:14:26 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/12 12:04:47 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_shell_script(const char *path)
{
	int		file;
	char	shebang[3];

	file = open(path, O_RDONLY, 0644);
	if (file == -1)
		return (ERROR);
	ft_memset(shebang, 0, 3);
	read(file, shebang, sizeof(shebang));
	shebang[2] = '\0';
	close(file);
	return (ft_strncmp(shebang, "#!", ft_strlen("shebang")));
}

static int	is_binary_executable(const char *path)
{
	int		file;
	char	buf[4];

	file = open(path, O_RDONLY, 0644);
	if (file == -1)
		return (ERROR);
	ft_memset(buf, 0, 4);
	close(file);
	return (ft_memcmp(buf,
						"\x7f"
						"ELF",
						4));
}

int	is_executable(const char *path)
{
	if (access(path, X_OK) != 0)
		return (FALSE);
	if (is_binary_executable(path) == 0)
		return (TRUE);
	else if (is_shell_script(path) == 0)
		return (TRUE);
	return (0);
}

int	is_in_charset(char c, const char *charset)
{
	if (!charset || !c)
		return (0);
	while (*charset)
		if (c == *charset++)
			return (1);
	return (0);
}

int	is_dir(const char *path, t_shell *data)
{
	int	temp;
	DIR	*fd;

	fd = opendir(path);
	if (fd == NULL)
	{
		temp = open(path, O_RDONLY);
		if (errno == EACCES)
		{
			data->last_status = 126;
			return (126);
		}
		else
		{
			data->last_status = 127;
			return (127);
		}
		write(2, " ", 1);
		ft_putendl_fd(strerror(errno), 2);
		if (temp != -1)
			close(temp);
	}
	else
	{
		data->last_status = 126;
		ft_putendl_fd(" Is a directory\n", 2);
		closedir(fd);
		return (126);
	}
}

void	handle_command_error(t_shell *sh, t_command *cmd, int err)
{
	if (is_in_charset(cmd->argv[0][0], "./") == 1
        && is_executable(cmd->argv[0]) == FALSE)
	err = is_dir(cmd->argv[0], sh);
	if (err == 127)
		ft_printf_error(ERR_MINISHELL_CMD_NOT_FOUND, cmd->argv[0]);
	else if (err == 126)
		ft_printf_error(ERR_MINISHELL_PERMISSION, cmd->argv[0]);
	free_shell(sh);
	exit(err);
}
