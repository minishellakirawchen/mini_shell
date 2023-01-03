# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/01 15:09:17 by takira            #+#    #+#              #
#    Updated: 2023/01/03 10:48:59 by wchen            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

# CC and CFLAGS
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -MMD -MP


# SRCS and OBJS
VPATH = $(SRC_DIR)

SRC_DIR	= ./srcs
SRC		= main.c \
		  input/input.c \
		  input/get_env.c \
		  execution/execute.c \
		  exit/exit.c

SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR	= ./objs
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

# DEPS	= $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))
DEPS	= $(OBJ:%.o=%.d)

# LFLAGS
LIB_DIR				= ./libs
LIBFT_DIR			= $(LIB_DIR)/libft
LIBFT				= $(LIBFT_DIR)/libft.a

LIBGNL_DIR			= $(LIB_DIR)/libgnl
LIBGNL				= $(LIBGNL_DIR)/libgnl.a

LIBFT_PRINTF_DIR	= $(LIB_DIR)/libftprintf
LIBFT_PRINTF		= $(LIBFT_PRINTF_DIR)/libftprintf.a

LIBS_DIR			= $(LIB_DIR) $(LIBGNL_DIR) $(LIBFT_PRINTF_DIR)
LFLAGS				= $(addprefix -L, $(LIBS_DIR)) -lreadline
LIBS				= $(LIBFT) $(LIBGNL) $(LIBFT_PRINTF)

# IFLAGS
INCLUDES_DIR = ./includes
IFLAGS = $(addprefix -I, $(INCLUDES_DIR))


# RULES
$(NAME)	: $(OBJS)
	@make -C $(LIBFT_DIR)
	@make -C $(LIBGNL_DIR)
	@make -C $(LIBFT_PRINTF_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(LIBS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

all	: $(NAME)

clean :
	rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(LIBGNL_DIR)
	@make clean -C $(LIBFT_PRINTF_DIR)

fclean : clean
	rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(LIBGNL_DIR)
	@make fclean -C $(LIBFT_PRINTF_DIR)

re : fclean all

.PHONY: all clean fclean re

-include	$(DEPS)
