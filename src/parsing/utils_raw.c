/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_raw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 00:41:40 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/11 00:41:51 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_raw(t_raw **head, t_raw **last, char *value, t_raw_type type)
{
	t_raw	*node;

	node = malloc(sizeof(t_raw));
	if (!node)
		return (ERROR);
	node->value = value;
	node->type = type;
	node->next = NULL;
	if (!*head)
		*head = node;
	else
		(*last)->next = node;
	*last = node;
	return (SUCCESS);
}

void	free_raw_tokens(t_raw *raw_tokens)
{
	t_raw	*cur;
	t_raw	*next;

	cur = raw_tokens;
	while (cur)
	{
		next = cur->next;
		free(cur->value);
		free(cur);
		cur = next;
	}
}
