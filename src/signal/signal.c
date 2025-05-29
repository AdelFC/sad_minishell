/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:57:59 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/29 14:26:20 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	if (rl_line_buffer)
		rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_sig = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(1, "Quit: 3\n", 8);
	g_sig = 131;
}

void	b_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
