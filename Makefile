GREEN := $(shell tput -Txterm setaf 2)
YELLOW := $(shell tput -Txterm setaf 3)
RED := $(shell tput -Txterm setaf 1)
BLUE := $(shell tput -Txterm setaf 4)
CYAN := $(shell tput -Txterm setaf 6)
RESET := $(shell tput sgr0)

NAME := ft_malcolm
CC := gcc
WARNINGS := -Wall -Wextra -Werror -pedantic -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
DEBUG := -g -ggdb #-fsanitize=address
CFLAGS := $(WARNINGS) $(DEBUG) -lpcap -I./include -MMD
LDFLAGS := -L./Libft -lft
LIBFT := ./Libft/libft.a

SRC_DIR := src
OBJ_DIR := obj
SRC := $(SRC_DIR)/main.c \
	   $(SRC_DIR)/utils.c \
	   $(SRC_DIR)/ft_signals.c \
	   $(SRC_DIR)/ft_set_dev.c \
	   $(SRC_DIR)/ft_arp.c \
	   $(SRC_DIR)/ft_send_arp.c \
	   $(SRC_DIR)/ft_reset_arp.c \
	   $(SRC_DIR)/ft_arp_bonus.c
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEPS := $(OBJ:.o=.d)


all: makelibs $(NAME)

makelibs:
	@echo "$(CYAN) 🛠  Compiling Libft... $(RESET)"
	@$(MAKE) -C Libft
	@echo "$(GREEN) ✅ Libft compiled successfully! $(RESET)"

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $@
	@echo "$(GREEN) ✅ $(NAME) compiled successfully! $(RESET)"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW) 🛠  Compiling $<... $(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

bonus: CFLAGS += -D BONUS
bonus: re

build:
	docker build -t arp -f Dockerfile .

run:
	docker-compose -f docker-compose.yaml up

down:
	docker-compose -f docker-compose.yaml down

clean:
	@echo "$(RED) 🧹 Cleaning object files... $(RESET)"
	@$(MAKE) -C Libft clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED) 🧹 Cleaning $(NAME)... $(RESET)"
	@$(MAKE) -C Libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
