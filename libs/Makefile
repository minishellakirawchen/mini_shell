# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/16 09:32:11 by takira            #+#    #+#              #
#    Updated: 2023/01/16 09:32:13 by takira           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror


FT_DIR		= libft
GNL_DIR		= libgnl
PRINTF_DIR	= libftprintf

FT_SRC		=


$(NAME)	: $(OBJS)


all	: $(NAME)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re	: fclean all

.PYONY : all clean fclean re