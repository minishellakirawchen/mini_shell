/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:30:52 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 12:34:54 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_chdir_path(char *current_path, char *move_to)
{
	size_t	current_len = ft_strlen_ns(current_path);
	size_t	move_to_len = ft_strlen_ns(move_to);
	char	*new_path;
	char 	*trimmed_move_to;

	if (move_to_len == 0)
		return (current_path);
	if (move_to[0] == '/')
		return (move_to);
	trimmed_move_to = ft_strtrim(move_to, "/");// /bin + hoge/ -> /bin + hoge
	if (!trimmed_move_to)
	{
		perror("malloc");
		return (NULL);
	}
	move_to_len = ft_strlen_ns(trimmed_move_to);
	new_path = (char *)ft_calloc(sizeof(char), current_len + move_to_len + 2);
	if (!new_path)
		return (NULL);
	ft_strlcat(new_path, current_path, current_len + 1);
	ft_strlcat(new_path, "/", current_len + 2);
	ft_strlcat(new_path, trimmed_move_to, current_len + move_to_len + 2);
	free(trimmed_move_to);// memcpy in strlcat
	return (new_path);
}

int ft_cd(t_info *info, char **cmds)
{
	int		chdir_ret;
	char 	*new_path;
	char 	*move_to;
	char 	*current_path;

	if (!info || !cmds || !cmds[1])
		return (EXIT_FAILURE); //TODO: exit?
	move_to = cmds[1];
	current_path = get_current_path();
	new_path = get_chdir_path(current_path, move_to);
	if (!new_path)
	{
//		free(current_path);
		perror("malloc");
		return (EXIT_FAILURE); //TODO: exit?
	}
	chdir_ret = chdir(new_path);
	if (chdir_ret < 0)
	{
		ft_printf("cd: no such file or directory: %s", move_to);//TODO: STDERROR
		return (CHDIR_FAILURE);
	}
	if (overwrite_env_value(&info->env_list, PWD, new_path) == FAILURE)
	{
		free(new_path);
		return (EXIT_FAILURE);
	}
	free(new_path);
	return (EXIT_SUCCESS);
}
