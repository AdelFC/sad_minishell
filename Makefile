CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CLIBS = -lreadline
NAME = minishell

FT_PRINTF_DIR = lib/ftprintf
FT_PRINTF_ERROR_DIR = lib/ft_printf_error
GNL_DIR = lib/get_next_line
LIBFT_DIR = lib/libft

SRC_DIR = src
OBJ_DIR = obj
INCS = -I includes

PARSER_DIR = $(SRC_DIR)/parsing
EXEC_DIR = $(SRC_DIR)/exec
SIGNAL_DIR = $(SRC_DIR)/signal
BUILTIN_DIR = $(SRC_DIR)/builtin
UTILS_DIR = $(SRC_DIR)/utils

SRC_FILES = minishell.c
PARSER_FILES = prompter.c parser.c valid_quotes.c clean_line.c tokens.c \
		utils_token.c raw_tokens.c post_process_tokens.c get_token_type.c \
		utils_functions.c utils_raw.c check_syntax.c error_syntax.c \
		utils_error_syntax.c expand_tokens.c expand_tokens2.c utils_extract_raw.c \
		split_commands.c build_commands.c utils_build_commands.c expand_utils.c \
		clean_line_U.c build_command2.c post_process_utils1.c post_process_utils2.c \
		clean_cmd_list.c
EXEC_FILES = commands_setup.c utils_handle_cmd_error.c \
		redirections.c heredoc.c pipex.c exec.c path.c \
		handle_command_error.c pipex_utils.c exec_utils.c \
		exec_utils2.c new_env.c pipexfirst.c
SIGNAL_FILES = signal.c
BUILTIN_FILES = exec_builtin.c ft_cd.c ft_echo.c ft_env.c \
		ft_exit.c ft_export.c ft_pwd.c ft_unset.c ft_cd_env.c \
		ft_export_utils.c
UTILS_FILES = env_setup.c shell_setup.c utils_parsing.c \
		utils_main.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
	   $(addprefix $(PARSER_DIR)/, $(PARSER_FILES)) \
	   $(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
	   $(addprefix $(SIGNAL_DIR)/, $(SIGNAL_FILES)) \
	   $(addprefix $(BUILTIN_DIR)/, $(BUILTIN_FILES)) \
	   $(addprefix $(UTILS_DIR)/, $(UTILS_FILES)) 

OBJS = $(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.o, $(subst $(SRC_DIR)/,,$(SRC))))

LIBS = -L$(FT_PRINTF_DIR) -lftprintf \
       -L$(FT_PRINTF_ERROR_DIR) -lftprintferror \
       -L$(GNL_DIR) -lftgnl \
       -L$(LIBFT_DIR) -lft

GREEN = \033[0;92m
BLUE = \033[0;94m
CYAN = \033[0;96m
YELLOW = \033[0;93m

all: $(FT_PRINTF_DIR)/libftprintf.a $(FT_PRINTF_ERROR_DIR)/libftprintferror.a $(GNL_DIR)/libgnl.a $(LIBFT_DIR)/libft.a $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(CLIBS) -o $(NAME)
	@echo "$(GREEN) Compilation of $(NAME) completed!$(BLUE)"

$(FT_PRINTF_DIR)/libftprintf.a:
	@make -C $(FT_PRINTF_DIR) --no-print-directory

$(FT_PRINTF_ERROR_DIR)/libftprintferror.a:
	@make -C $(FT_PRINTF_ERROR_DIR) --no-print-directory

$(GNL_DIR)/libgnl.a:
	@make -C $(GNL_DIR) --no-print-directory

$(LIBFT_DIR)/libft.a:
	@make -C $(LIBFT_DIR) --no-print-directory

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@
	@echo "$(CYAN)Compiled: $<$(BLUE)"

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(FT_PRINTF_DIR) clean --no-print-directory
	@make -C $(FT_PRINTF_ERROR_DIR) clean --no-print-directory
	@make -C $(GNL_DIR) clean --no-print-directory
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@echo "$(YELLOW)Cleaned object files.$(BLUE)"

fclean: clean
	@rm -f $(NAME)
	@make -C $(FT_PRINTF_DIR) fclean --no-print-directory
	@make -C $(FT_PRINTF_ERROR_DIR) fclean --no-print-directory
	@make -C $(GNL_DIR) fclean --no-print-directory
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "$(YELLOW)Removed $(NAME) and compiled files.$(BLUE)"

re: fclean all

fast:
	@$(MAKE) --no-print-directory fclean > /dev/null 2>&1
	@$(MAKE) --no-print-directory all    > /dev/null 2>&1
	@echo "$(GREEN) Minishell compiled!"

.PHONY: all clean fclean re
