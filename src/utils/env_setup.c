/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:55:07 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 15:45:59 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(const char *name, const char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->name = ft_strdup(name);
    if (!node->name)
	{
		free(node);
		return (NULL);
	}
    node->value = ft_strdup(value);
    if (!node->value)
	{
		free(node->name);
		free(node);
		return (NULL);
	}
    node->next = NULL;
    return (node);
}

static void	init_env_var_utils(t_env_utils *env_var)
{
    env_var->eq = NULL;
    env_var->name = NULL;
    env_var->value = NULL;
    env_var->node = NULL;
    env_var->i = 0;
}

static void	fill_env_var_utils(t_env_utils *env_var, char *env)
{
    env_var->eq = ft_strchr(env, '=');
    if (!env_var->eq)
        return;
    env_var->name = ft_substr(env, 0, env_var->eq - env);
    env_var->value = ft_strdup(env_var->eq + 1);
    env_var->node = new_env(env_var->name, env_var->value);
    free(env_var->name);
    free(env_var->value);
}

/*
 * Initializes a linked list of environment variables from the envp array
 */

t_env	*init_env(char **envp)
{
    t_env	*head;
    t_env	*tail;
    t_env_utils env_var;

    head = NULL;
    tail = NULL;
    init_env_var_utils(&env_var);
    while (envp[env_var.i])
    {
        fill_env_var_utils(&env_var, envp[env_var.i]);
        if (!env_var.eq)
        {
            env_var.i++;
            continue;
        }
        if (!env_var.node)
            return (NULL);
        if (!head)
            head = env_var.node;
        else
            tail->next = env_var.node;
        tail = env_var.node;
        env_var.i++;
    }
    return (head);
}

/*
** Properly frees the memory allocated for the t_env structure
*/

void	free_env(t_env *env)
{
	t_env	*cur;
	t_env	*next;

	cur = env;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		cur = next;
	}
}
