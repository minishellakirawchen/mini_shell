/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:30:52 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 20:30:55 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_new_path(char *current_path, char *move_to)
{
	size_t	current_len = ft_strlen_ns(current_path);
	size_t	move_len = ft_strlen_ns(move_to);
	char	*new_path;

	if (move_len == 0)
		return (current_path);
	if (move_to[0] == '/')
		return (move_to);
	new_path = (char *)ft_calloc(sizeof(char), current_len + move_len + 2);
	if (!new_path)
		return (NULL);
	ft_strlcat(new_path, current_path, current_len + 1);
	ft_strlcat(new_path, "/", current_len + 2);
	ft_strlcat(new_path, move_to, current_len + move_len + 2);
	return (new_path);
}

int ft_cd(t_info *info)
{
	int		chdir_ret;
	char 	*new_path;
	char 	*move_to;
	char 	*current_path;

	if (!info || !info->commands[1])
		return (1); //TODO: exit?
	move_to = info->commands[1];
	current_path = get_current_path();
	new_path = get_new_path(current_path, move_to);
	if (!new_path)
	{
//		free(current_path);
		perror("malloc");
		return (1); //TODO: exit?
	}
	chdir_ret = chdir(new_path);
	if (chdir_ret < 0)
	{
		ft_printf("cd: no such file or directory: %s", move_to);//TODO: STDERROR
		return (CHDIR_FAILURE);
	}
	// TODO: search env PWD and change it.
	return (0);
}
