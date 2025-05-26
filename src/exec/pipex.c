/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:10:11 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/26 16:24:39 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
** Enfant : exécute la première commande d'un pipeline
*/
static void process_first(t_command *cmd, t_shell *sh)
{
    char *path;
    int  status;

    if (apply_redirections(cmd->redirs) == ERROR)
        exit(EXIT_FAILURE);

    if (dup2(cmd->fd[1], STDOUT_FILENO) == -1)
    {
        ft_printf_error(ERR_DUP2, strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(cmd->fd[0]);
    close(cmd->fd[1]);

    if (is_builtin(cmd->argv[0]) == ERROR)
    {
        status = exec_builtin(cmd->argv, &sh->env);
        exit(status);
    }

    /* recherche du binaire puis execve */
    path = find_path(cmd->argv[0], sh->envp);
    if (!path)
        handle_command_error(cmd, 127);

    execve(path, cmd->argv, sh->envp);
    /* si retour, c’est une erreur de permission */
    free(path);
    handle_command_error(cmd, 126);
}

/*
** Enfant : commande intermédiaire (stdin ← prev_fd, stdout → cmd->fd[1])
*/
static void process_middle(int prev_fd, t_command *cmd, t_shell *sh)
{
    char *path;
    int  status;

    if (apply_redirections(cmd->redirs) == ERROR)
        exit(EXIT_FAILURE);
    if (dup2(prev_fd, STDIN_FILENO) == -1 || dup2(cmd->fd[1], STDOUT_FILENO) == -1)
    {
        ft_printf_error(ERR_DUP2, strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(cmd->fd[0]);
    close(cmd->fd[1]);
    close(prev_fd);
    if (is_builtin(cmd->argv[0]) == ERROR)
    {
        status = exec_builtin(cmd->argv, &sh->env);
        exit(status);
    }
    path = find_path(cmd->argv[0], sh->envp);
    if (!path)
        handle_command_error(cmd, 127);

    execve(path, cmd->argv, sh->envp);
    free(path);
    handle_command_error(cmd, 126);
}

/*
** Enfant : dernière commande (stdin ← prev_fd)
*/
static void process_last(int prev_fd, t_command *cmd, t_shell *sh)
{
    char *path;
    int  status;

    if (apply_redirections(cmd->redirs) == ERROR)
        exit(EXIT_FAILURE);
    if (dup2(prev_fd, STDIN_FILENO) == -1)
    {
        ft_printf_error(ERR_DUP2, strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(prev_fd);
    if (is_builtin(cmd->argv[0]) == ERROR)
    {
        status = exec_builtin(cmd->argv, &sh->env);
        exit(status);
    }
    path = find_path(cmd->argv[0], sh->envp);
    if (!path)
        handle_command_error(cmd, 127);
    execve(path, cmd->argv, sh->envp);
    free(path);
    handle_command_error(cmd, 126);
}

/*
** ft_pipe : orchestre les forks et pipes pour toute la liste sh->cmds
*/
void ft_pipe(t_shell *sh, int *last_status)
{
    int        prev_fd = -1;
    pid_t      pid;
    t_command *cur = sh->cmds;

    while (cur)
    {
        if (cur->next && pipe(cur->fd) < 0)
            ft_printf_error(ERR_PIPE);
        pid = fork();
        if (pid < 0)
        {
            ft_printf_error(ERR_FORK);
        }
        else if (pid == 0)
        {
            if (!cur->next)
                process_last(prev_fd, cur, sh);
            else if (prev_fd == -1)
                process_first(cur, sh);
            else
                process_middle(prev_fd, cur, sh);
        }
        if (cur->next)
        {
            close(cur->fd[1]);
            if (prev_fd != -1)
                close(prev_fd);
            prev_fd = cur->fd[0];
        }
        cur = cur->next;
    }
    while (wait(last_status) > 0)
        continue;
}
