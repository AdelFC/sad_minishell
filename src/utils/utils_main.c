/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:55:56 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 14:56:20 by afodil-c         ###   ########.fr       */
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
	if (ac > 1)
		return (ft_printf_error("minishell: too many arguments\n"), ERROR);
	return (SUCCESS);
}
