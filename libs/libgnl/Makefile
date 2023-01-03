# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/26 21:08:21 by takira            #+#    #+#              #
#    Updated: 2022/11/26 21:08:22 by takira           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			= cc

CFLAGS		= -Wall -Wextra -Werror

NAME		= libgnl.a

SRCS		= get_next_line.c \
			get_next_line_utils.c

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
	ar rcs $(NAME) $(OBJS)

bonus:		all

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
