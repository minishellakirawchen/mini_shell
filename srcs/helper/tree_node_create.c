/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:58:03 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:50:37 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_2d_array_size(const char **array)
{
	size_t	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	is_redirect_sign(const char *cmd)
{
	return (is_same_str(cmd, REDIRECT_IN) \
	|| is_same_str(cmd, REDIRECT_OUT) \
	|| is_same_str(cmd, REDIRECT_HEREDOC) \
	|| is_same_str(cmd, REDIRECT_APPEND));
}

ssize_t	get_update_cmds_size(const char **cmds)
{
	ssize_t	size;
	size_t	idx;

	if (!cmds)
		return (0);
	size = 0;
	idx = 0;
	while (cmds[idx])
	{
		if (is_redirect_sign(cmds[idx]))
		{
			idx++;
			if (!cmds[idx])
				return (-1);
		}
		else
			size++;
		idx++;
	}
	return (size);
}

// delete elems -> cmds[i]:redirect && cmd[i+1]:file or here_doc
char	**update_cmds(const char **cmds)
{
	ssize_t	size;
	size_t	dst_idx;
	size_t	src_idx;
	char	**update_cmds;

	errno = 0;
	size = get_update_cmds_size(cmds);
	if (!cmds || size < 0)
		return (NULL);
	update_cmds = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!update_cmds)
		return (NULL);
	dst_idx = 0;
	src_idx = 0;
	while (cmds[src_idx])
	{
		if (is_redirect_sign(cmds[src_idx]))
		{
			src_idx += 2;
			continue ;
		}
		update_cmds[dst_idx] = ft_strdup_ns(cmds[src_idx]);
		if (!update_cmds[dst_idx])
		{
			free_2d_array_ret_nullptr((void ***)&update_cmds);
			return (perror_and_return_nullptr("malloc"));
		}
		src_idx++;
		dst_idx++;
	}
	return (update_cmds);
}

t_tree	*create_tree_node(t_exe_type type, const char **cmds)
{
	t_tree		*new_node;

	errno = 0;
	new_node = (t_tree *)malloc(sizeof(t_tree));
	if (!new_node)
		return (perror_and_return_nullptr("malloc"));
	new_node->exe_type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->redirect_info = NULL;
	if (!cmds)
	{
		new_node->cmds = NULL;
		return (new_node);
	}
	new_node->redirect_info = get_redirection_info(cmds);
	if (!new_node->redirect_info)
	{
		free_1d_array_ret_nullptr((void **)&new_node);
		return (NULL);
	}
	new_node->cmds = update_cmds(cmds);
	if (!new_node->cmds)
	{
		free_1d_array_ret_nullptr((void **)&new_node);
		return (NULL);
	}
//	debug_print_2d_arr(new_node->cmds, "update cmds      ");
	return (new_node);
}
