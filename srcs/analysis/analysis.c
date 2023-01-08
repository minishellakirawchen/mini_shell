/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 20:09:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まずpipeでsplitしたものをexec_stackに入れていく
// その際にsplie(space), trim(space)して整形する
// flg_in = redirectin, here_doc, stdin
// flg_out = redirect_out, append, stdout

int	add_command_leaf_to_node(t_tree **node, char *command_line)
{
	char 	**split_by_pipe;
	t_tree	*cmd_leaf;
	size_t	idx;

	split_by_pipe = ft_split_set(command_line, '|', '"');//TODO: update split set; char set-> char *set
	if (!split_by_pipe)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
//	debug_print_2d_arr(split_by_pipe, "split by pipe");

	idx = 0;
	while (split_by_pipe[idx])
	{
		cmd_leaf = create_tree_node(E_LEAF_COMMAND, split_by_pipe[idx]);
		if (!cmd_leaf)
			return (perror_and_return_int("malloc", EXIT_FAILURE));
		add_bottom_of_tree(node, cmd_leaf);
		idx++;
	}
	return (SUCCESS);
}

// First try :: create commands which connected 1 level pipe, like: $> cat Makefile | grep a | grep b
//  -> CLEAR!!!
// 2nd try   :: pipe in subshell				, like: $> (cat Makefile | grep a)
// 3rd try   :: subshell in pipeline			, like: $> cat Makefile | (cd /bin) | echo hello
// 4th try   :: pipe in subshell in pipeline	, like: $> cat Makefile | (echo hello | grep h) | ls -l
int	analysis(t_info *info)
{
	t_tree	*root_node;
	t_tree 	*pipe_node;

	if (!info)
		return (FAILURE);
	// create exe-elem "root"
	root_node = create_tree_node(E_NODE_ROOT, NULL);
	if (!root_node)
		return (FAILURE); // TODO:free
	add_bottom_of_tree(&info->tree_root, root_node);

	// create exec-elem "pipe" and create edge to root
	pipe_node = create_tree_node(E_NODE_PIPE, NULL);
	add_bottom_of_tree(&info->tree_root, pipe_node); //root, pipe共通のelemを代入すると...?
	/* create exe-elem "commands" which connected same level pipes */
	//   [root]              <- root node
	//     |
	//   [pipe]              <- pipe node (explain execute stage)
	//     |_______ .... _
	//     |     |       |
	//   [cmd1][cmd2]  [cmdn]  <- command leaf (execute args)
	//                             **args = {"cmd1", "cmd2", .., "cmdn", NULL}
	//                             -> ft_execvp(args[0], args, NULL)
	add_command_leaf_to_node(&pipe_node, info->input_line); //input: tmp
	// if fail -> all free, use tree_clear

	// BFS的な実装で入れ子でも順番に展開していける？

//	debug_print_stack(info->tree_root, "check pipe case");
	return (SUCCESS);
}
