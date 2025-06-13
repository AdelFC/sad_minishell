/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:31:50 by barnaud           #+#    #+#             */
/*   Updated: 2025/06/13 15:32:50 by barnaud          ###   ########.fr       */
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
