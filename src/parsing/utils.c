/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:38:51 by barnaud           #+#    #+#             */
/*   Updated: 2025/05/29 12:42:27 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	**find_pp(t_token **head, t_token *tok)
{
	t_token	**pp;

	pp = head;
	while (*pp && *pp != tok)
		pp = &(*pp)->next;
	return (pp);
}

void	free_words(char **words)
{
	int	i;

	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
}

t_token	*split_and_insert_tokens(t_token **head, t_token *tok, char *str)
{
	t_split_utils	u;

	u.words = ft_split(str, ' ');
	if (!u.words)
		return (NULL);
	u.pp = find_pp(head, tok);
	u.first = NULL;
	u.last = NULL;
	u.i = 0;
	build_tokens(&u, tok);
	if (u.last)
	{
		u.last->next = tok->next;
		*(u.pp) = u.first;
	}
	free(tok->value);
	free(tok);
	free_words(u.words);
	if (u.last)
		return (u.last->next);
	return (NULL);
}

int	is_expandable_token(t_token *tok)
{
	return (tok->type != T_PIPE && tok->type != T_INFILE_OPERATOR
		&& tok->type != T_OUTFILE_OPERATOR && tok->type != T_APPEND_OPERATOR
		&& tok->type != T_HEREDOC_OPERATOR);
}

t_token	*process_expansion(t_token **head, t_token *tok, t_env *env_list,
		int last_status)
{
	char	*old;
	char	*new;

	old = tok->value;
	new = expand_string(old, env_list, last_status);
	if (!new)
		return (tok->next);
	if (!tok->was_quoted && ft_strchr(new, ' '))
	{
		tok = split_and_insert_tokens(head, tok, new);
		free(new);
		return (tok);
	}
	free(old);
	tok->value = new;
	return (tok->next);
}
