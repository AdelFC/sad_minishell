/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:13 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 19:17:14 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if s is a valid identifier: [A-Za-z_][A-Za-z0-9_]*
*/
static int is_valid_identifier(const char *s)
{
    int i;

    if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
        return (ERROR);
    i = 1;
    while (s[i])
    {
        if (!(ft_isalnum(s[i]) || s[i] == '_'))
            return (ERROR);
        i++;
    }
    return (SUCCESS);
}

/*
** Frees name and value pair
*/
static void free_name_value(char *name, char *value)
{
    free(name);
    free(value);
}

/*
** Export a single "name" or "name=value"
*/
static int export_single(t_env **env, const char *arg)
{
    char *name;
    char *value;
    int   pos;

    pos = 0;
    while (arg[pos] && arg[pos] != '=')
        pos++;
    name = ft_substr(arg, 0, pos);
    if (!name)
        return (ERROR);
    if (arg[pos] == '=')
        value = ft_strdup(arg + pos + 1);
    else
        value = ft_strdup("");
    if (!value)
        return (free(name), ERROR);
    if (is_valid_identifier(name) == ERROR)
    {
        ft_printf_error(ERR_EXPORT_VALID_ID, name);
        free_name_value(name, value);
        return (ERROR);
    }
    if (env_set(env, name, value) == ERROR)
    {
        free_name_value(name, value);
        return (ERROR);
    }
    free_name_value(name, value);
    return (SUCCESS);
}

/*
** Loop over argv[1..] and export each
*/
static int export_loop(char **argv, t_env **env)
{
    int ret;
    int res;
    int i;
    
    i = 1;
    ret = SUCCESS;
    while (argv[i])
    {
        res = export_single(env, argv[i]);
        if (res == ERROR)
            ret = ERROR;
        i++;
    }
    return (ret);
}

static void init_t_sort_alpha(t_sort_alpha *st, t_env *env)
{
    st->sorted     = NULL;
    st->cur        = env;
    st->next       = NULL;
    st->insert_pos = NULL;
    st->len_cur    = 0;
    st->len_ins    = 0;
    st->max_len    = 0;
}

static t_env *ft_sort_alpha(t_env *env)
{
    t_sort_alpha sort;

    init_t_sort_alpha(&sort, env);
    while (sort.cur)
    {
        sort.next       = sort.cur->next;
        sort.insert_pos = &sort.sorted;
        while (*sort.insert_pos)
        {
            sort.len_cur = ft_strlen(sort.cur->name);
            sort.len_ins = ft_strlen((*sort.insert_pos)->name);
            if (sort.len_cur > sort.len_ins)
                sort.max_len = sort.len_cur;
            else
                sort.max_len = sort.len_ins;
            if (ft_strncmp(sort.cur->name,
                           (*sort.insert_pos)->name,
                           sort.max_len) > 0)
                sort.insert_pos = &(*sort.insert_pos)->next;
            else
                break;
        }
        sort.cur->next   = *sort.insert_pos;
        *sort.insert_pos = sort.cur;
        sort.cur         = sort.next;
    }
    return (sort.sorted);
}

/*
** Export builtin:
** - Without args  → print all env "name=value"
** - With args     → export each
*/
int ft_export(char **argv, t_env **env)
{
    int i;
    t_env *cur;

    i = 1;
    if (!argv[i])
    {
        cur = ft_sort_alpha(*env);
        while (cur)
        {
            if (cur->value)
                printf("export %s=\"%s\"\n", cur->name, cur->value);
            cur = cur->next;
        }
        return (SUCCESS);
    }
    return (export_loop(argv, env));
}
