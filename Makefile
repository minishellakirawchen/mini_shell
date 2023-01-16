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
		  analysis/analysis.c \
		  analysis/split_pipe.c \
		  analysis/split_redirection.c \
		  analysis/split_helper.c \
		  analysis/analysis_redirect.c \
		  analysis/tokenize.c \
		  analysis/create_tree.c \
		  expansion/expand_variable.c \
		  expansion/arrangement_command_line.c \
		  execution/execute.c \
		  execution/execute_builtin.c \
		  execution/execute_redirect.c \
		  execution/execute_heredoc.c \
		  execution/fork_wait_helpers.c \
		  execution/handle_filedes.c \
		  execution/ft_execve.c \
		  execution/ft_execvp.c \
		  execution/execute_pipe_iterative.c \
		  exit/exit.c \
		  builtin/ft_echo.c \
		  builtin/ft_cd.c \
		  builtin/ft_pwd.c \
		  builtin/ft_export.c \
		  builtin/ft_unset.c \
		  builtin/ft_env.c \
		  builtin/ft_exit.c \
		  helper/tree_node_create.c \
		  helper/tree_operation.c \
		  helper/tree_helper.c \
		  helper/debug_print.c \
		  helper/env_list.c \
		  helper/is_same_str.c \
		  signal/signal.c \


SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR	= ./objs
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRC:%.c=%.o))

# DEPS	= $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))
DEPS	= $(SRC:%.c=%.d)

# LFLAGS
LIB_DIR				= ./libs
LIBFT_DIR			= $(LIB_DIR)/libft
LIBFT				= $(LIBFT_DIR)/libft.a

LIBGNL_DIR			= $(LIB_DIR)/libgnl
LIBGNL				= $(LIBGNL_DIR)/libgnl.a

LIBFT_PRINTF_DIR	= $(LIB_DIR)/libftprintf
LIBFT_PRINTF		= $(LIBFT_PRINTF_DIR)/libftprintf.a

READLINE_DIR		= $(shell brew --prefix readline)/lib

LIBS_DIR			= $(LIB_DIR) $(LIBGNL_DIR) $(LIBFT_PRINTF_DIR) $(READLINE_DIR)

LFLAGS				= $(addprefix -L, $(LIBS_DIR)) -lreadline
LIBS				= $(LIBFT) $(LIBGNL) $(LIBFT_PRINTF)


# IFLAGS
INCLUDES_DIR	= ./includes
INCLUDES		= $(INCLUDES_DIR) $(shell brew --prefix readline)/include
IFLAGS			= $(addprefix -I, $(INCLUDES))


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

norm :
	norminette -v
	norminette $(SRC_DIR) $(LIBS_DIR) $(INCLUDE_DIR)

.PHONY: all clean fclean re norm

-include	$(DEPS)
