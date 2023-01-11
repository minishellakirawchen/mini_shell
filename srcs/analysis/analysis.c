/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/10 18:42:48 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まずpipeでsplitしたものをexec_stackに入れていく
// その際にsplie(space), trim(space)して整形する
// flg_in = redirectin, here_doc, stdin
// flg_out = redirect_out, append, stdout

int	valid_input(char **pipe_splitted_input)
{
	size_t	idx;
	bool	is_error;

	idx = 0;
	is_error = false;
	while (pipe_splitted_input[idx])
	{
		if (pipe_splitted_input[idx][0] == '|')
		{
			if (idx == 0 || !pipe_splitted_input[idx + 1])
				is_error = true;
			if (ft_strlen_ns(pipe_splitted_input[idx]) > 1)
				is_error = true;
			if (pipe_splitted_input[idx + 1] && pipe_splitted_input[idx + 1][0] == '|')
				is_error = true;
			if (is_error)
			{
				printf("minishell: syntax error near unexpected token `%s'\n", pipe_splitted_input[idx]);// TODO:STDERROR
				return (FAILURE);
			}
		}
		idx++;
	}
	return (SUCCESS);
}

// First try :: create commands which connected 1 level pipe, like: $> cat Makefile | grep a | grep b
//  -> CLEAR!!!
// 2nd try   :: pipe in subshell				, like: $> (cat Makefile | grep a)
// 3rd try   :: subshell in pipeline			, like: $> cat Makefile | (cd /bin) | echo hello
// 4th try   :: pipe in subshell in pipeline	, like: $> cat Makefile | (echo hello | grep h) | ls -l
int	analysis(t_info *info, char *readline_input)
{
	if (!info)
		return (FAILURE);

	// split input by before or after of '|'
	// * input [echo hello |grep a| echo "hello | world" >out]
	// * split {"echo", "hello", "|", "grep", "a", "|", "echo", "hello", "|", "world", ">out", NULL}
	info->splitted_cmds = split_pipe_and_word_controller(readline_input);
	if (!info->splitted_cmds)
		return (perror_and_return_int("malloc", EXIT_FAILURE));

	// valid_input
	// * syntax error : {"|", "echo", "hello || ", "||", "cat", "Makefile", "|", "|", NULL}
	//                    ^ first one                ^ |x2                        ^ | next of | or last one
	if (valid_input(info->splitted_cmds) == FAILURE)
	{
		free_2d_array_ret_nullptr((void ***)&info->splitted_cmds);
		return (SYNTAX_ERROR);
	}
	debug_print_2d_arr(info->splitted_cmds, "pipe_splitted    ");


	// split redirect sign
	info->splitted_cmds = split_redirect_and_word_controller((const char **)info->splitted_cmds);
	if (!info->splitted_cmds)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
	// valid redirect sign

	// * syntax error : {"cat", "Makefile", "<", "file", ">>>", "", "", "<", "<" NULL}
	//
	//                                                     ^ >x3              ^ continues, no file
	if (valid_redirection((const char **)info->splitted_cmds) == FAILURE)
		return (SYNTAX_ERROR);

//	debug_print_2d_arr(info->splitted_cmds, "redirect_splitted");

	// create tree
	if (create_tree(&info) == FAILURE)
	{
		free_info(&info); //TODO:error handling
		return (EXIT_FAILURE);
	}

	// check tree
	//
	//   [root]                 <- root node
	//     |
	//   [pipe]                 <- pipe node (explain execute stage)
	//     |_______ .... _
	//     |     |       |
	//   [cmd1] [cmd2]  [cmdn]  <- command leaf (execute args)
	//                             cmd1 = {"cmd11", "cmd12", "cmd13",.., NULL} -> ft_execvp(cmd1[0], cmd1, NULL)
	//                             cmd2 = {"cmd21", "cmd22", "cmd23",.., NULL}    execve("echo", {"echo", "hello", "world", NULL}, NULL)
	//
//	printf("#print tree\n");
	debug_print_stack(info->tree_root, "check tree");

//	add_command_leaf_to_node(&pipe_node, info->readline_input); //input: tmp
	// if fail -> all free, by tree_clear

	// BFS的な実装で入れ子でも順番に展開していける？

	return (SUCCESS);
}
