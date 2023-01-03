# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/01 15:09:17 by takira            #+#    #+#              #
#    Updated: 2023/01/01 15:09:21 by takira           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

# CC and CFLAGS
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -MMD -MP


# SRCS and OBJS
SRC_DIR	= ./srcs
SRC		= main.c \
		  input/input.c \
		  input/get_env.c \
		  execution/execute.c \
		  exit/exit.c

SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR	= ./objs
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))

DEPS	= $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

# LFLAGS
LIB_DIR				= ./libs
LIBFT_DIR			= $(LIB_DIR)/libft
LIBGNL_DIR			= $(LIB_DIR)/libgnl
LIBFT_PRINTF_DIR	= $(LIB_DIR)/libftprintf
LIBS_DIR			= $(LIB_DIR) $(LIBGNL_DIR) $(LIBFT_PRINTF_DIR)
LFLAGS	= $(addprefix -L, $(LIBS_DIR))

# IFLAGS
INCLUDE_DIR = ./includes
IFLAGS = $(addprefix -I, $(INCLUDE_DIR))


# RULES
$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(IFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

all	: $(NAME)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re

-include	$(DEPS)