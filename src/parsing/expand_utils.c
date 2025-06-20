/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:31:50 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/20 10:10:03 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_char(char *str, char c)
{
	size_t	len;
	char	*result;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
	{
		free(str);
		return (NULL);
	}
	if (str)
		ft_memcpy(result, str, len);
	result[len] = c;
	result[len + 1] = '\0';
	free(str);
	return (result);
}

char	*get_to_join(const char *var_value, int *allocated)
{
	char	*to_join;

	if (!var_value)
	{
		to_join = ft_strdup("");
		*allocated = 1;
	}
	else
	{
		to_join = ft_strdup(var_value);
		*allocated = 1;
	}
	return (to_join);
}
