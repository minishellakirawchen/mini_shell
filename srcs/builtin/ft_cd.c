/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:30:52 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 21:54:05 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_chdir_path(char *current_path, char *move_to)
{
	const size_t	current_len = ft_strlen_ns(current_path);
	size_t			move_to_len;
	char			*new_path;
	char 			*trimmed_move_to;

	errno = 0;
	move_to_len = ft_strlen_ns(move_to);
	if (move_to_len == 0)
		return (current_path);
	if (move_to[0] == '/')
		return (move_to);
	trimmed_move_to = ft_strtrim(move_to, "/"); // /bin + hoge/ -> /bin + hoge
	if (!trimmed_move_to)
		return ((char *) perror_and_return_nullptr("malloc"));
	move_to_len = ft_strlen_ns(trimmed_move_to);
	new_path = (char *)ft_calloc(sizeof(char), current_len + move_to_len + 2);
	if (!new_path)
		return (NULL);
	ft_strlcat(new_path, current_path, current_len + 1);
	ft_strlcat(new_path, "/", current_len + 2);
	ft_strlcat(new_path, trimmed_move_to, current_len + move_to_len + 2);
	free_1d_array_ret_nullptr((void **)&trimmed_move_to); // memcpy in strlcat
	return (new_path);
}

// TODO: check permission denied
int	ft_cd(t_info *info, const char **cmds)
{
	int		chdir_ret;
	char 	*new_path;
	char 	*move_to;
	char 	*current_path;

	errno = 0;
	if (!info || !cmds)
		return (EXIT_FAILURE); //TODO: exit?
	if (get_2d_array_size((const char **)cmds) == 1)
		return (EXIT_SUCCESS);
	move_to = (char *)cmds[1];
	current_path = getcwd(NULL, 0);
	if (!current_path)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	new_path = get_chdir_path(current_path, move_to);
	free_1d_array_ret_nullptr((void **)&current_path);
	if (!new_path)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	chdir_ret = chdir(new_path);
	free_1d_array_ret_nullptr((void **)&new_path);
	if (chdir_ret < 0)
	{
		ft_dprintf(STDERR_FILENO, "cd: no such file or directory: %s\n", move_to);
		return (CHDIR_FAILURE);
	}
	current_path = getcwd(NULL, 0);
	if (!current_path)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	if (overwrite_env_value(&info->env_list, PWD, current_path) == FAILURE)
	{
		free_1d_array_ret_nullptr((void **)&current_path);
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	}
	free_1d_array_ret_nullptr((void **)&current_path);
	return (EXIT_SUCCESS);
}
