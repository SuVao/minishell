NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I./inc
SRC_DIR = ./src
OBJ_DIR = ./obj
LIBFT_DIR = ./libft
LDFLAGS = -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

# Diretórios dos módulos
BUILTINS_DIR = $(SRC_DIR)/builtins
SIG_DIR = $(SRC_DIR)/signals
EXEC_DIR = $(SRC_DIR)/exec
PARSER_DIR = $(SRC_DIR)/parser
FREES_DIR = $(SRC_DIR)/frees

# Arquivos de origem com caminhos completos
SRC_FILES = $(SRC_DIR)/main.c \
			$(SRC_DIR)/shell_loop.c \
            $(BUILTINS_DIR)/cd.c \
            $(BUILTINS_DIR)/echo.c \
            $(BUILTINS_DIR)/pwd.c \
            $(BUILTINS_DIR)/env.c \
            $(BUILTINS_DIR)/exit.c \
            $(BUILTINS_DIR)/export.c \
            $(BUILTINS_DIR)/unset.c \
            $(SIG_DIR)/signal.c \
            $(EXEC_DIR)/execute_ast.c \
			$(EXEC_DIR)/execute_ast1.c \
			$(EXEC_DIR)/fill_node.c \
            $(EXEC_DIR)/create_ast.c \
            $(EXEC_DIR)/get_path.c \
            $(EXEC_DIR)/here_doc.c \
			$(EXEC_DIR)/parse_tokens.c \
			$(EXEC_DIR)/open_redirs.c \
			$(EXEC_DIR)/open_redirs1.c \
			$(EXEC_DIR)/heredoc_prio.c \
			$(EXEC_DIR)/close_fds.c \
            $(PARSER_DIR)/tokenize.c \
            $(PARSER_DIR)/exp_tokens.c \
            $(PARSER_DIR)/slice_tokens.c \
            $(PARSER_DIR)/find_operators.c \
            $(PARSER_DIR)/quotes.c \
			$(PARSER_DIR)/exp_tokens_0.c \
			$(FREES_DIR)/free_args.c \
			$(FREES_DIR)/free_ast.c \
			$(FREES_DIR)/free_heredocs.c \
			$(BUILTINS_DIR)/cd2.c \
            $(BUILTINS_DIR)/env2.c \
            $(BUILTINS_DIR)/env3.c \
            $(BUILTINS_DIR)/export2.c \


OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	@echo "Compiling minishell..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	@echo "Compiling libft..."
	@make -C $(LIBFT_DIR)

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "Executable removed."

re: fclean all

.PHONY: all clean fclean re
