/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:23:58 by barnaud           #+#    #+#             */
/*   Updated: 2025/05/29 14:36:48 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_file(int fd)
{
	struct stat	st;

	if (fstat(fd, &st) == -1)
		return (0);
	return (S_ISREG(st.st_mode));
}

int	init_basics(int ac, char **av)
{
	(void)av;
	if (!isatty(STDIN_FILENO) || is_pipe(STDIN_FILENO) || is_pipe(STDOUT_FILENO)
		|| is_file(STDIN_FILENO) || is_file(STDOUT_FILENO))
		return (ft_printf_error("minishell: cannot run a pipe \
            or with redirections\n"),
			ERROR);
	if (ac > 1)
		return (ft_printf_error("minishell: too many arguments\n"), ERROR);
	return (SUCCESS);
}
