/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_u.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:00:13 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/20 10:09:43 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
}

void	restore_signals_parent(void)
{
	signal(SIGINT, handle_sigint);
}
