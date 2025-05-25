/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:36:55 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 00:01:47 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	if (c == 32 || c == 9)
		return (SUCCESS);
	return (ERROR);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (SUCCESS);
	return (ERROR);
}

int	skip_spaces(char *line)
{
	int	i;

	i = 0;
	while (is_space(line[i]) == SUCCESS)
		i++;
	return (i);
}

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
