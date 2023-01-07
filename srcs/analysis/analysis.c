/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 14:39:01 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まずpipeでsplitしたものをexec_stackに入れていく
// その際にsplie(space), trim(space)して整形する

char **splitset_and_trim(char *src, char delim, char set, char *trimchar)
{
	char	**splitted_strs;
	char	*trimmed_str;
	size_t	i;

	splitted_strs = ft_split_set(src, delim, set);
	if (!splitted_strs)
		return (NULL);
	i = 0;
	while (splitted_strs[i])
	{
		trimmed_str = ft_strtrim(splitted_strs[i], trimchar);
		if (!trimmed_str)
			return (free_array(splitted_strs));
		free(splitted_strs[i]);
		splitted_strs[i] = ft_strdup(trimmed_str);
		free(trimmed_str);
		i++;
	}
//	debug_print_2d_arr(splitted_strs, "splited_str[i]");
	return (splitted_strs);
}

t_tree	*create_tree_node(t_exe_type type, char *raw_cmd_str)
{
	t_tree		*new_node;

	new_node= (t_tree *)malloc(sizeof(t_tree));
	if (!new_node)
	{
		perror("malloc");
		return (NULL);
	}
	new_node->exe_type = type;
	if (!raw_cmd_str)
	{
		new_node->cmds = NULL;
		return (new_node);
	}
	new_node->cmds = splitset_and_trim(raw_cmd_str, ' ', '"', ISSPACE);
	if (!new_node->cmds)
	{
		perror("malloc");
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

// First try, create commands which connected 1 level pipe, like: $> cat Makefile | grep a | grep b
int	analysis(t_info *info)
{
	t_tree	*root_node;
	t_tree 	*pipe_node;
	t_tree	*cmd_node;
	char 	**split_by_pipe;
	size_t	i;

	if (!info)
		return (FAILURE);

	// create exe-elem "root"
	root_node = create_tree_node(E_ROOT, NULL);
	if (!root_node)
		return (FAILURE); // TODO:free
	add_bottom_of_tree(&info->exe_root, root_node);

	// create exec-elem "pipe" and create edge to root
	pipe_node = create_tree_node(E_PIPE, NULL);
	add_bottom_of_tree(&info->exe_root, pipe_node); //root, pipe共通のelemを代入すると...?

	// create exe-elem "commands" which connected same level pipes

	// root
	//  |
	// pipe
	//  |_______ .. _
	//  |     |     |
	// cmd1  cmd2  cmdn

	split_by_pipe = ft_split_set(info->input_line, '|', '"');
	if (!split_by_pipe)
		return (FAILURE); // TODO:free
//	debug_print_2d_arr(split_by_pipe, "split by pipe");

	i = 0;
	while (split_by_pipe[i])
	{
		cmd_node = create_tree_node(E_CMD, split_by_pipe[i]);
		if (!cmd_node)
		{
			tree_clear(&info->exe_root);
			return (FAILURE);
		}
		add_bottom_of_tree(&info->exe_root, cmd_node);
		i++;
	}
	debug_print_stack(info->exe_root, "print stack");
	// TODO: 任意のnodeにぶら下げるには...?
	return (SUCCESS);
}
