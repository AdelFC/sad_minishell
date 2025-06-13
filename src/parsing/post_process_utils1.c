/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_process_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:58:21 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 16:00:21 by barnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_assignment_case(t_ppraw_utils *raw)
{
	raw->value = join_and_free(raw->key, raw->val);
	if (!raw->value)
	{
		free_ppraw_utils(raw);
		return (ERROR);
	}
	raw->key = NULL;
	raw->val = NULL;
	return (SUCCESS);
}

int	handle_assignment_or_quotes(t_ppraw_utils *raw, t_raw *cur)
{
	char	*tmp;

	if (split_assignment_quoted(cur->value, &raw->key, &raw->val,
			&raw->was_quoted) == SUCCESS)
		return (handle_assignment_case(raw));
	raw->value = ft_strdup(cur->value);
	if (!raw->value)
	{
		free_ppraw_utils(raw);
		return (ERROR);
	}
	tmp = remove_outer_quotes(raw->value, &raw->was_quoted);
	if (!tmp)
	{
		free(raw->value);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	raw->value = tmp;
	return (SUCCESS);
}

int	check_token_creation_error(t_ppraw_utils *raw, t_token **tokens_out)
{
	if (!raw->token)
	{
		free(raw->value);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	if (raw->type == T_ARG && raw->value == NULL)
	{
		free(raw->token->value);
		free(raw->token);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	if (!tokens_out || !raw->token)
	{
		free(raw->token->value);
		free(raw->token);
		free_ppraw_utils(raw);
		return (ERROR);
	}
	return (SUCCESS);
}

int	create_and_add_token(t_ppraw_utils *raw, t_token **tokens_out)
{
	if (raw->was_quoted)
		raw->type = T_ARG;
	else
		raw->type = get_token_type(raw->value, tokens_out);
	raw->token = create_token(raw->type, raw->value, raw->was_quoted);
	if (check_token_creation_error(raw, tokens_out) == ERROR)
		return (ERROR);
	add_token(tokens_out, raw->token);
	raw->value = NULL;
	return (SUCCESS);
}
