/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:14:26 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 10:15:47 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_shell_script(const char *path)
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

int	is_binary_executable(const char *path)
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