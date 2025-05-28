/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afodil-c <afodil-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:36:58 by afodil-c          #+#    #+#             */
/*   Updated: 2025/05/28 23:05:05 by afodil-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*===== LIBS =====*/

# include "../lib/ft_printf_error/ft_printf_error.h"
# include "../lib/ftprintf/ft_printf.h"
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

/*===== STRUC & CONST =====*/

# define SUCCESS 0
# define ERROR -1

# define ERR_MINISHELL_CMD_NOT_FOUND "minishell: %s: command not found\n"
# define ERR_MINISHELL_PERMISSION "minishell: %s: permission denied\n"

# define ERR_CD_HOME_NOT_SET "minishell: cd: HOME not set\n"
# define ERR_CD_TOO_MANY_ARGS "minishell: cd: too many arguments\n"
# define ERR_CD_CWD "minishell: could not open working directory\n"
# define ERR_CD_NO_SUCH_DIR "minishell: cd: %s: No such file or directory\n"

# define ERR_PWD_TOO_MANY_ARGS "pwd: too many arguments\n"
# define ERR_PWD_CANNOT_CWD "pwd: cannot access current directory\n"

# define ERR_UNSET_NO_ARGS "unset: not enough arguments\n"
# define ERR_UNSET_NOT_FOUND "unset: did not find var\n"
# define ERR_EXPORT_VALID_ID "export: %s: not a valid identifier\n"

# define ERR_UNCLOSED_QUOTE "error: unclosed quote\n"

# define ERR_TOKEN_PIPE "minishell: syntax error near unexpected token `|`\n"
# define ERR_DS "minishell: syntax error near unexpected token `;;`\n"
# define ERR_TPC "minishell: syntax error near unexpected token `)`\n"
# define ERR_TPO "minishell: syntax error near unexpected token `(`\n"
# define ERR_TN "minishell: syntax error near unexpected token `newline`\n"
# define ERR_UT "minishell: syntax error near unexpected token `%s`\n"

# define ERR_DUP2 "minishell: dup2: %s\n"
# define ERR_PIPE "minishell: pipe failed\n"
# define ERR_FORK "minishell: fork failed\n"

# define ERR_ENV_ARG "minishell: env: too many arguments\n"
# define ERR_ENV_OPTIONS "minishell: env: options not supported\n"
# define ERR_NO_ENV "minishell: no env detected\n"

# define ERR_EXIT_TOO_MANY_ARGS "exit: too many arguments\n"
# define ERR_EXIT_NUM_ARG "exit: numeric argument required\n"

# define ERR_EXPORT_VALID_ID "export: %s: not a valid identifier\n"

typedef enum e_types
{
	T_CMD = 1,
	T_ARG,
	T_PIPE,
	T_INFILE_FILE,
	T_INFILE_OPERATOR,
	T_OUTFILE_FILE,
	T_OUTFILE_OPERATOR,
	T_APPEND_FILE,
	T_APPEND_OPERATOR,
	T_HEREDOC_DELIM,
	T_HEREDOC_OPERATOR
}						t_e_types;

typedef enum e_syntax_error
{
	ERR_PIPE_START = 1,
	ERR_PIPE_END,
	ERR_DOUBLE_PIPE,
	ERR_REDIR_NEWLINE,
	ERR_REDIR_MISSING,
	ERR_DOUBLE_SEMI,
	ERR_PAREN_UNMATCHED
}						t_syntax_error;

typedef enum e_redir_type
{
	REDIR_IN = 4,
	REDIR_OUT = 6,
	REDIR_APPEND = 8,
	REDIR_HDOC = 10,
}						t_redir_type;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_token
{
	int					type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	int					type;
	char				*filename;
	int					heredoc_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**argv;
	t_redir				*redirs;
	int					fd[2];
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	int					last_status;
	t_env				*env;
	char				**envp;
	t_token				*tokens;
	t_command			*cmds;
}						t_shell;

typedef struct s_heredoc_utils
{
	char				filename[256];
	int					fd_write;
	int					fd_read;
	char				*pid_str;
	char				*cnt_str;
	int					count;
}						t_heredoc_utils;

/*UTILS_STRUC*/

typedef struct s_env_utils
{
	char				*eq;
	char				*name;
	char				*value;
	t_env				*node;
	int					i;
}						t_env_utils;

typedef struct s_clean_line
{
	char				*res;
	char				*line;
	int					i;
	int					j;
	int					space;
	int					in_squote;
	int					in_dquote;
}						t_clean_line;

typedef struct s_expand_tok
{
	const char			*str;
	t_env				*env_list;
	int					last_status;
	int					i;
	int					in_sq;
	int					in_dq;
	char				*res;
}						t_expand_tok;

typedef struct s_sort_alpha
{
	t_env				*sorted;
	t_env				*cur;
	t_env				*next;
	t_env				**insert_pos;
	size_t				len_cur;
	size_t				len_ins;
	size_t				max_len;
}						t_sort_alpha;

typedef struct s_sort_utils
{
	int					swapped;
	size_t				len;
	t_env				*curr;
}						t_sort_utils;

/*===== UTILS =====*/
/*env_setup.c*/
t_env					*new_env(const char *name, const char *value);
t_env					*init_env(char **envp);
void					free_env(t_env *env);

/*shell_setup.c*/
t_shell					*init_shell(char **envp);
void					free_shell(t_shell *sh);

/*utils_parsing.c*/
int						is_space(char c);
int						is_operator(char c);
int						skip_spaces(char *line);
char					*ft_strjoin_nobug(const char *s1, const char *s2);
char					*join_and_free(char *res, char *seg);

/*===== PARSING =====*/
/*tokens.c*/
void					free_tokens(t_token *head);
int						tokenize(char *line, t_token **tokens);

/*clean_line.c*/
char					*clean_line(char *line);

/*clean_line_utils.c*/
void					init_clean_line_vars(t_clean_line *var, char *line);

/*get_next_token.c*/
char					*get_next_token(const char *line, int *i);

/*get_token_type.c*/
int						get_token_type(char *value, t_token **head);

/*prompter.c*/
char					*init_prompt(void);

/*parser.c*/
int						parse_line(char *line, t_shell *sh);

/*valid_quotes.c*/
int						valid_quotes(char *line);

/*check_syntax.c*/
int						check_syntax(t_token *tokens);

/*error_syntax.c*/
int						error_pipe_start_end(t_token *tokens);
int						error_double_pipe(t_token *tokens);
int						error_redirection(t_token *tokens);
int						error_double_semicolon(t_token *tokens);
int						error_parenthesis(t_token *tokens);
int						is_pipe(int type);
int						is_redir_op(int type);
int						is_redir_file(int type);

/* expand_utils.c */
void					init_expand_tok(t_expand_tok *v, const char *s,
							t_env *env_list, int last_status);
char					*extract_var(t_expand_tok *v);
char					*get_env_value(t_env *env_list, const char *key);
char					*build_new_res(const char *old, const char *val);
void					append_char(t_expand_tok *v, char c);

/* expand_tokens.c */
char					*expand_string(const char *s, t_env *env_list,
							int last_status);
void					expand_tokens(t_token *tok, t_env *env_list,
							int last_status);

/*===== BUILTINS =====*/

/*exec_builtin.c*/
int						is_builtin(const char *cmd);
int						exec_builtin(char **argv, t_shell *sh);

/*ft_cd.c*/
int						ft_cd(char **argv, t_env **env);

/*ft_cd_env.c*/
char					*env_get(t_env *env, const char *name);
int						env_set(t_env **env, const char *name,
							const char *value);

/*ft_echo.c*/
int						ft_echo(char **argv);

/*ft_exit.c*/
int						ft_exit(char **argv, t_shell *sh);

/*ft_env.c*/
int						ft_env(char **argv, t_env **env);

/*ft_export.c*/
t_env					*sort_env_alpha(t_env *env);
int						is_valid_identifier(const char *s);
char					*get_var_name(const char *arg);
int						export_with_value(const char *arg, t_env **env);
int						ft_export(char **argv, t_env **env);

/*ft_pwd.c*/
int						ft_pwd(char **argv);

/*ft_unset.c*/
int						ft_unset(char **argv, t_env **env);

/*===== EXECUTION =====*/
/*commands_setup.c*/
void					free_commands(t_command *cmds);

/*commands_utils.c*/
t_command				*new_command(void);
int						add_argv(t_command *cmd, const char *arg);
int						add_redir(t_command *cmd, int type,
							const char *filename);

/*commands.c*/
t_command				*build_commands_from_tokens(t_token *tokens);

/*handle_command_error.c*/
void					handle_command_error(t_shell *sh, t_command *cmd,
							int err);

/*exec.c*/
int						exec_commands(t_shell *sh);
void					cleanup_heredocs_list(t_command *cmd);

/*path.c*/
char					*join_path(const char *dir, const char *cmd);
char					*find_path(const char *cmd, char **envp);

/*redirections.c*/
int						apply_redirections(t_redir *redir);

/*heredoc.c*/
void					ctrl_c_heredoc(int sig);
void					handle_heredoc_input(t_shell *sh, const char *limiter, int fd);
int						handle_heredoc(const char *limiter, t_shell *sh);

/*pipex.c*/

void					process_first(t_command *cmd, t_shell *sh);
void					setup_pipes(t_command *cmd, int prev_fd);
void					process_middle(int prev_fd, t_command *cmd,
							t_shell *sh);
void					process_last(int prev_fd, t_command *cmd, t_shell *sh);
void					update_prev_fd(int *prev_fd, t_command *cur);
void					handle_pipe_iteration(t_command *cur, t_shell *sh,
							int prev_fd);
void					ft_pipe(t_shell *sh, int *last_status);

/*===== SIGNAL =====*/

extern int				g_sig;

void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					init_signals(void);

#endif
