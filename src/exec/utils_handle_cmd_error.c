/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handle_cmd_error.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:12:48 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 10:13:03 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_charset(char c, const char *charset)
{
	if (!charset || !c)
		return (0);
	while (*charset)
		if (c == *charset++)
			return (1);
	return (0);
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
