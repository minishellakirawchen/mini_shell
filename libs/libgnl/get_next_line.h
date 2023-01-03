/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 19:09:48 by takira            #+#    #+#             */
/*   Updated: 2022/11/19 14:49:06 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdbool.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 42

char	*get_next_line(int fd, bool is_include_nl);
char	*strjoin_and_free_dst(char *dst, char *src);

void	*ft_free(char **alloc1, char **alloc2);

size_t	ft_strlen_gnl(const char *str);
size_t	cnt_chr_in_str(const char c, const char *str);
size_t	ft_strlcpy_gnl(char *dst, const char *src, size_t dstsize);

#endif
