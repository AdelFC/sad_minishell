/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:14:26 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 16:15:47 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_error(t_command *cmd, int err)
{
	if (err == 127)
		ft_printf_error(ERR_MINISHELL_CMD_NOT_FOUND, cmd->argv[0]);
	else if (err == 126)
		ft_printf_error(ERR_MINISHELL_PERMISSION, cmd->argv[0]);
	free_commands(cmd);
	exit(err);
}
