/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:57:59 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 11:41:11 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	init_signals(void)
{
	struct sigaction	saa;

	saa.sa_handler = handle_sigint;
	sigemptyset(&saa.sa_mask);
	saa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &saa, NULL) == -1)
		ft_printf_error("minishell: sigaction(SIGINT) failed\n");
	saa.sa_handler = SIG_IGN;
	sigemptyset(&saa.sa_mask);
	saa.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &saa, NULL) == -1)
		ft_printf_error("minishell: sigaction(SIGQUIT) failed\n");
}
