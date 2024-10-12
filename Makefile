
NAME= minishell

CC= cc
CFLAGS= -Wall -Wextra -Werror
SRC_DIR= ./src
OBJ_DIR= ./obj
LDFLAGS = -lreadline

SRC_FILES= main.c\

SRCS= $(addprefix $(SRC_DIR)/,$(SRC_FILES))
OBJ= $(addprefix $(OBJ_DIR)/,$(SRC_FILES:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "minishell compiled"

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "minishell object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "minishell removed"

re: fclean all

.PHONY: all clean fclean re
