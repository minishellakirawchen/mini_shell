/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 12:25:22 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 16:06:37 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// * split {"echo", "hello", "|", "grep", "a", "|", "echo", "hello", "|", "cat", "Makefile", ">", "out", NULL}
// * node  root
//          |_ pipe
//               |_ command {"echo", "hello", NULL}
//               |_ command {"grep", "a", NULL}
//               |_ command {"cat", "Makefile", ">", "out", NULL}
//               |_ NULL

size_t	get_pipe_idx(char **cmds)
{
	size_t	idx;

	if (!cmds)
		return (0);
	idx = 0;
	while (cmds[idx])
	{
		if (is_same_str(cmds[idx], "|"))
		{
//			printf("same:cmds[%zu]:%s\n", idx, cmds[idx]);
			return (idx);
		}
		idx++;
	}
	return (idx);
}

size_t	count_pipe(char **cmds)
{
	size_t	idx;
	size_t	cnt;

	if (!cmds)
		return (0);
	idx = 0;
	cnt = 0;
	while (cmds[idx])
	{
		if (is_same_str(cmds[idx], "|"))
			cnt++;
		idx++;
	}
	return (cnt);
}

char **get_dup_cmds(char **src, size_t start_idx, size_t size)
{
	char	**line;
	size_t	i;

	if (!src)
		return (NULL);
	i = 0;
	while (src[i])
		i++;
	if (i < start_idx || i < start_idx + size)
		return (NULL);
	line = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!line)
		return ((char **)perror_and_ret_nullptr("malloc"));
	i = 0;
	while (i < size)
	{
		line[i] = ft_strdup_ns(src[start_idx + i]);
		if (!line[i])
		{
			free_2d_array_ret_nullptr((void ***)&line);
			return (perror_and_ret_nullptr("malloc"));
		}
		i++;
	}
	return (line);
}

int	create_tree(t_info **info)
{
	size_t	head_idx;
	size_t	size;
	t_tree	*root_node;
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	char	**cmd_line;

	if (!info || !*info || !(*info)->splitted_cmds)
		return (FAILURE);

	// create root node
	root_node = create_tree_node(E_NODE_ROOT, NULL);
	if (!root_node)
		return (perror_and_return_int("malloc", FAILURE)); // TODO:free
	add_bottom_of_tree(&(*info)->tree_root, root_node);
	// create pipe or shell node
	pipe_node = create_tree_node(E_NODE_PIPE, NULL);
	if (!pipe_node)
		return (perror_and_return_int("malloc", FAILURE)); // TODO:free
	if (count_pipe((*info)->splitted_cmds) == 0)
		pipe_node->exe_type = E_NODE_SHELL;
	add_bottom_of_tree(&(*info)->tree_root, pipe_node);

	// new_nodeを使い回しても上書きされない？関数にcopyが渡されるから？ TODO:check
	head_idx = 0;
	while ((*info)->splitted_cmds[head_idx])
	{
		size = get_pipe_idx(&(*info)->splitted_cmds[head_idx]);
		cmd_line = get_dup_cmds((*info)->splitted_cmds, head_idx, size);
		if (!cmd_line)
			return (perror_and_return_int("malloc", FAILURE)); // TODO:free

		debug_print_2d_arr(cmd_line, "debug cmd_line");

		cmd_node = create_tree_node(E_LEAF_COMMAND, (const char **)cmd_line);
		if (!cmd_node)
			return (perror_and_return_int("malloc", FAILURE)); // TODO:free
		add_bottom_of_tree(&(*info)->tree_root, cmd_node);
		head_idx += size;
		if (is_same_str((*info)->splitted_cmds[head_idx], "|"))
			head_idx++;
	}
	return (SUCCESS);
}
