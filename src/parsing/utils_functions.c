/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:59:20 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/13 13:38:46 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_nobug(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*new;
	size_t	i;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new = malloc(len1 + len2 + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		new[i] = s1[i];
		i++;
	}
	while (i - len1 < len2)
	{
		new[i] = s2[i - len1];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*join_and_free(char *res, char *seg)
{
	char	*tmp;

	tmp = ft_strjoin_nobug(res, seg);
	free(res);
	free(seg);
	return (tmp);
}
