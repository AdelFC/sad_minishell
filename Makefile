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

SRC_FILES = minishell.c
PARSER_FILES = 
EXEC_FILES = 
SIGNAL_FILES = 
BUILTIN_FILES = 

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
	   $(addprefix $(PARSER_DIR)/, $(PARSER_FILES)) \
	   $(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
	   $(addprefix $(SIGNAL_DIR)/, $(SIGNAL_FILES)) \
	   $(addprefix $(BUILTIN_DIR)/, $(BUILTIN_FILES)) 

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

.PHONY: all clean fclean re
