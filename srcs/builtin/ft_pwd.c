/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:01 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 21:49:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
// Free src inside ft_realloc
// If len(src) > newsize, terminating of new_ptr is undefined
void	*ft_realloc(void *src, size_t newsize)
{
	void	*new_ptr;
	size_t	size;

	size = ft_strlen_ns(src);
	if (size == newsize)
		return (src);
	new_ptr = (void *)malloc(sizeof(void) * newsize);
	ft_memcpy(new_ptr, src, newsize);
	free(src);
	return (new_ptr);
}

//TODO: use getcdw(NULL), use malloc for justsize
char *get_current_path(void)
{
	char	*path;
	size_t	size;

	size = PATH_MAX;
	path = (char *)malloc(sizeof(char) * size);
	if (!path)
	{
		perror("malloc");
		return (NULL);
	}
	while (!getcwd(path, size))
	{
		size++;
		path = (char *)ft_realloc(path, size);
//		printf("path:%s, size:%zu, len:%zu\n", path, size, ft_strlen_ns(path));
		if (!path)
		{
			perror("malloc");
			return (NULL);
		}
	}
//	printf("path:%s, size:%zu, len:%zu\n", path, size, ft_strlen_ns(path));
	return (path);
}
*/

int	ft_pwd(t_info *info)
{
	char	*current_path;

	errno = 0;
	if (!info)
		return (1); //TODO: exit?
	current_path = getcwd(NULL, 0);
	if (errno != 0)
		return (perror_and_return_int("malloc", FAILURE));
	ft_printf("%s\n", current_path);
	free(current_path);
	return (0);
}
