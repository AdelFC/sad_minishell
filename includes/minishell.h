/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barnaud <barnaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 21:36:58 by afodil-c          #+#    #+#             */
/*   Updated: 2025/06/16 12:45:19 by barnaud          ###   ########.fr       */
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
# define ERR_MINISHELL_PERMISSION "minishell: %s: Permission denied\n"

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
	int					was_quoted;
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

typedef struct s_env_utils
{
	char				*eq;
	char				*name;
	char				*value;
	t_env				*node;
	int					i;
}						t_env_utils;

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

typedef struct s_split_utils
{
	char				**words;
	t_token				**pp;
	t_token				*first;
	t_token				*last;
	int					i;
}						t_split_utils;

/*===== UTILS =====*/
/*utils_main*/
int						is_file(int fd);
int						init_basics(int ac, char **av);

/*env_setup.c*/
t_env					*new_env(const char *name, const char *value);
t_env					*init_env(char **envp);
void					free_env(t_env *env);

/*shell_setup.c*/
t_shell					*init_shell(char **envp);
void					free_shell(t_shell *sh);

/*===== PARSING =====*/

/*prompter.c*/
char					*init_prompt(void);

/*parser.c*/
int						parse_line(char *line, t_shell *sh);

/*valid_quotes.c*/
int						valid_quotes(char *line);

/*clean_line.c*/
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

char					*clean_line(char *line);
void					process_operator(t_clean_line *var);
void					process_regular_char(t_clean_line *var);
void					char_buffer(char *res, int *j, char c);

/*utils_functions.c*/
char					*ft_strjoin_nobug(const char *s1, const char *s2);
char					*join_and_free(char *res, char *seg);

/*tokens.c*/
t_token					*create_token(int type, char *value, int was_quoted);
void					add_token(t_token **head, t_token *new);
int						new_token(char *value, int was_quoted,
							t_token **tokens);
int						tokenize(char *line, t_token **tokens);

/*get_token_type.c*/
int						get_token_type(char *value, t_token **head);

/*raw_tokens.c*/
typedef enum e_raw_type
{
	RAW_WORD,
	RAW_OPERATOR
}						t_raw_type;

typedef struct s_raw
{
	char				*value;
	t_raw_type			type;
	struct s_raw		*next;
}						t_raw;

typedef struct s_raw_utils
{
	t_raw				*head;
	t_raw				*last;
	int					i;
}						t_raw_utils;

int						raw_tokenize(const char *line, t_raw **raw_tokens);
char					*extract_raw_operator(const char *line, int *i);
char					*extract_raw_token(const char *line, int *i);
char					*extract_raw_quoted(const char *line, int *i);
char					*extract_raw_word_segment(const char *line, int *i);
int						append_raw(t_raw **head, t_raw **last, char *value,
							t_raw_type type);
void					free_raw_tokens(t_raw *raw_tokens);

/*post_process_tokens.c*/
typedef struct s_remove_quote_utils
{
	int					in_sq;
	int					in_dq;
	int					i;
	int					j;
	int					last_quote;
	int					len;
	char				*out;
}						t_remove_quote_utils;

typedef struct s_ppraw_utils
{
	char				*value;
	int					was_quoted;
	char				*key;
	char				*val;
	int					type;
	t_token				*token;
}						t_ppraw_utils;

int						post_process_raw(t_raw *raw_tokens,
							t_token **tokens_out);
int						split_assignment_quoted(const char *raw_value,
							char **assign_key, char **assign_val,
							int *was_quoted);
char					*remove_outer_quotes(char *str, int *was_quoted);

/*post_process_utils.c*/
int						handle_assignment_case(t_ppraw_utils *raw);
int						handle_assignment_or_quotes(t_ppraw_utils *raw,
							t_raw *cur);
int						check_token_creation_error(t_ppraw_utils *raw,
							t_token **tokens_out);
int						create_and_add_token(t_ppraw_utils *raw,
							t_token **tokens_out);
void					free_ppraw_utils(t_ppraw_utils *raw);
void					init_remove_quote_utils(t_remove_quote_utils *rq,
							char *str);
int						get_assignment_indices(const char *raw_value,
							int *eq_idx, int *quote_idx, char *quote);
void					remove_outer_quotes_loop(t_remove_quote_utils *rq,
							char *str, int *was_quoted);

/*token_utils.c*/
void					free_tokens(t_token *tokens);

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

/*expand_tokens.c*/
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

void					expand_tokens(t_token **tokens, t_env *env_list,
							int last_status);
void					expand_token_value(t_token *token, t_env *env_list,
							int last_status);
void					handle_dollar_sign(t_expand_tok *v, char **result);
void					handle_single_quotes(const char *str, char **result,
							int *i);
char					*strjoin_char(char *str, char c);
void					init_expand_tok(t_expand_tok *v, const char *s,
							t_env *env_list, int last_status);
char					*get_env_value(t_env *env_list, const char *key);

/*split_commands.c*/
t_token					**split_commands(t_token *tokens);
void					free_cmd_splits(t_token **cmd_splits);

/*clean_cmd_list.c*/
void					clean_leading_empty_argv(t_command *cmd);

/*build_commands.c*/
t_command				*build_commands_from_splits(t_token **cmd_splits);
int						handle_simple_token(t_command **cur, t_token *tok);
int						handle_pipe(t_command **current);

/*utils_build_commands.c*/
t_command				*new_command(void);
int						add_argv(t_command *cmd, const char *arg);
int						add_redir(t_command *cmd, int type,
							const char *filename);

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

/*handle_command_error.c*/
void					handle_command_error(t_shell *sh, t_command *cmd,
							int err);
int						is_shell_script(const char *path);
int						is_binary_executable(const char *path);
int						is_executable(const char *path);
int						is_dir(const char *path, t_shell *data);
int						is_in_charset(char c, const char *charset);

/*new_env.c*/
char					**env_list_to_array(t_env *env);
void					free_array(char **arr);

/*exec.c*/
int						exec_commands(t_shell *sh);
void					cleanup_heredocs_list(t_command *cmd);

/*path.c*/
char					*join_path(const char *dir, const char *cmd);
char					*find_path(const char *cmd, t_env *env);

/*redirections.c*/
int						apply_redirections(t_redir *redir);

/*heredoc.c*/
void					ctrl_c_heredoc(int sig);
void					handle_heredoc_input(t_shell *sh, const char *limiter,
							int fd);
int						handle_heredoc(const char *limiter, t_shell *sh);

/*pipex.c*/
void					end_f(t_command *cmd, t_shell *sh);
void					process_first(t_command *cmd, t_shell *sh);
void					setup_pipes(t_command *cmd, int prev_fd);
void					process_middle(int prev_fd, t_command *cmd,
							t_shell *sh);
void					process_last(int prev_fd, t_command *cmd, t_shell *sh);
void					update_prev_fd(int *prev_fd, t_command *cur);
void					handle_pipe_iteration(t_command *cur, t_shell *sh,
							int prev_fd);
void					ft_pipe(t_shell *sh, int *last_status);

/*exec_utils*/
void					wait_for_single(pid_t pid, t_shell *sh);
int						handle_redirections_and_save(t_command *cmd,
							t_shell *sh, int *in_save, int *out_save);
int						handle_exit_builtin(t_command *cmd, t_shell *sh,
							int in_save, int out_save);
void					handle_redir_without_cmd(t_command *cmd);
void					restore_std_fds(int in_save, int out_save);

/*utils_parsing.c*/
void					init_clean_line_vars(t_clean_line *var, char *line);
int						is_space(char c);
int						is_operator(char c);
int						skip_spaces(char *line);

/*===== SIGNAL =====*/

extern int				g_sig;

void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					init_signals(void);
void					b_sig(void);
void					sigpipe_handler(int sig);

#endif
