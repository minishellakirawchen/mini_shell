/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 09:18:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まずpipeでsplitしたものをexec_stackに入れていく
// その際にsplie(space), trim(space)して整形する
// flg_in = redirectin, here_doc, stdin
// flg_out = redirect_out, append, stdout

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
	root_node = create_tree_node(E_NODE_ROOT, NULL);
	if (!root_node)
		return (FAILURE); // TODO:free
	add_bottom_of_tree(&info->exe_root, root_node);

	// create exec-elem "pipe" and create edge to root
	pipe_node = create_tree_node(E_NODE_PIPE, NULL);
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
		cmd_node = create_tree_node(E_LEAF_COMMAND, split_by_pipe[i]);
		if (!cmd_node)
		{
			tree_clear(&info->exe_root);
			return (FAILURE);
		}
		add_bottom_of_tree(&info->exe_root, cmd_node);
		i++;
	}
	debug_print_stack(info->exe_root, "check pipe case");
	// TODO: 任意のnodeにぶら下げるには...?
	return (SUCCESS);
}
