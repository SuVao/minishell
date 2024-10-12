NAME= minishell

CC= cc
CFLAGS= -Wall -Wextra -Werror
SRC_DIR= ./src
OBJ_DIR= ./obj
LIBFT_DIR= ./Libft/
LDFLAGS = -lreadline
LIBFT = $(LIBFT_DIR)/libft.a
SRC_FILES= main.c\

SRCS= $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo "minishell compiled"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) -L$(LIBFT_DIR) -lft

$(LIBFT):
	@make -C $(LIBFT_DIR)

all: $(NAME)

clean:
	@make clean -C ${LIBFT_DIR}
	@rm -rf $(OBJ_DIR)
	@echo "minishell object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "minishell removed"

re: fclean all

.PHONY: all clean fclean re
