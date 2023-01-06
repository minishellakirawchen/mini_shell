# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/20 19:18:14 by takira            #+#    #+#              #
#    Updated: 2022/11/13 18:19:48 by takira           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc

CFLAGS		= -Wall -Wextra -Werror

NAME		= libft.a

SRCS		= ft_strlen.c \
				ft_strlcpy.c \
              	ft_strlcat.c \
              	ft_strchr.c \
              	ft_strrchr.c \
              	ft_strnstr.c \
              	ft_strncmp.c \
              	ft_memset.c \
              	ft_bzero.c \
              	ft_memcpy.c \
              	ft_memchr.c \
              	ft_memmove.c \
              	ft_memcmp.c \
              	ft_atoi.c \
              	ft_isalpha.c \
              	ft_isdigit.c \
              	ft_isalnum.c \
              	ft_isascii.c \
              	ft_isprint.c \
              	ft_toupper.c \
              	ft_tolower.c \
              	ft_calloc.c \
              	ft_strdup.c \
              	ft_substr.c \
              	ft_strjoin.c \
              	ft_strtrim.c \
              	ft_split.c \
              	ft_itoa.c \
              	ft_strmapi.c \
              	ft_striteri.c\
              	ft_putchar_fd.c \
              	ft_putstr_fd.c \
              	ft_putendl_fd.c \
              	ft_putnbr_fd.c \
              	ft_split_set.c \
              	ft_lstnew.c \
				ft_lstadd_front.c \
				ft_lstsize.c \
				ft_lstlast.c \
				ft_lstadd_back.c \
				ft_lstdelone.c \
				ft_lstclear.c \
				ft_lstiter.c \
				ft_lstmap.c \
				ft_math.c \
				ft_stack_operation.c \
				ft_stack_helper.c

OBJS		= $(SRCS:.c=.o)

$(NAME):	$(OBJS)
			ar rcs $(NAME) $(OBJS)

all:		$(NAME)

clean:
			rm -f $(OBJS) $(BONUSOBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re bonus
