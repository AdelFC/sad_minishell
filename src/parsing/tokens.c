/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:09:11 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 13:39:06 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(int type, char *value, int was_quoted)
{
    t_token *token = malloc(sizeof(t_token));

    if (!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->was_quoted = was_quoted;
    token->next = NULL;
    return (token);
}

void	add_token(t_token **head, t_token *new)
{
    t_token	*tmp;

    if (!*head)
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

int	new_token(char *value, int was_quoted, t_token **tokens)
{
    t_token	*token;
    int		type;

    type = get_token_type(value, tokens);
    token = create_token(type, value, was_quoted);
    if (!token)
        return (ERROR);
    add_token(tokens, token);
    return (SUCCESS);
}

int	tokenize(char *line, t_token **tokens)
{
    t_raw	*raws;
    int		status;

    *tokens = NULL;
    if (raw_tokenize(line, &raws) == ERROR)
        return (ERROR);
    status = post_process_raw(raws, tokens);
    free_raw_tokens(raws);
    if (status == ERROR)
    {
        if (*tokens)
            free_tokens(*tokens);
        *tokens = NULL;
        return (ERROR);
    }
    return (SUCCESS);
}

/*void debug_print_raw_tokens(t_raw *raws)
{
	 t_raw *curr = raws;
	 ft_printf("Raw tokens:\n");
	 while (curr)
	 {
		 ft_printf("  value: '%s'\n", curr->value);
		 curr = curr->next;
	 }
}

void debug_print_tokens(t_token *tokens)
{
	 t_token *curr = tokens;
	 ft_printf("Processed tokens:\n");
	 while (curr)
	 {
		 ft_printf("  type: %d, value: '%s'\n", curr->type, curr->value);
		 curr = curr->next;
	 }
}*/