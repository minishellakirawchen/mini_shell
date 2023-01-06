/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 18:27:46 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まず|でsplitしたものをexec_stackに入れていく

t_stack	*create_root_node(void)
{
	t_exe_elem	*exe_elem;
	t_stack		*root_node;

	exe_elem = (t_exe_elem *)malloc(sizeof(t_exe_elem));
	if (!exe_elem)
	{
		perror("malloc");
		return (NULL);
	}
	exe_elem->exe_type = E_ROOT;
	exe_elem->cmds = NULL;
	root_node = create_stack_elem(exe_elem);
	if (!root_node)
	{
		perror("malloc");
		return (NULL);
	}
	return (root_node);
}

t_stack	*create_pipe_node(void)
{
	t_exe_elem	*exe_elem;
	t_stack		*pipe_node;

	exe_elem = (t_exe_elem *)malloc(sizeof(t_exe_elem));
	if (!exe_elem)
	{
		perror("malloc");
		return (NULL);
	}
	exe_elem->exe_type = E_PIPE;
	exe_elem->cmds = NULL;
	pipe_node = create_stack_elem(exe_elem);
	if (!pipe_node)
	{
		perror("malloc");
		return (NULL);
	}
	return (pipe_node);
}

t_stack	*create_pipe_elem(char *cmd)
{
	t_exe_elem	*exe_elem;
	t_stack		*root;

	exe_elem = (t_exe_elem *)malloc(sizeof(t_exe_elem));
	if (!exe_elem)
	{
		perror("malloc");
		return (NULL);
	}
	exe_elem->exe_type = E_PIPE;
	exe_elem->cmds = ft_split_set(cmd, ' ', '"');
	root = create_stack_elem(exe_elem);
	if (!root)
	{
		perror("malloc");
		return (NULL);
	}
	return (root);
}

// First try, create commands which connected 1 level pipe, like: $> cat Makefile | grep a | grep b
int	analysis(t_info *info)
{
	t_stack		*exe_stack;
	char 		**split_by_pipe;
	size_t		i;

	if (!info)
		return (FAILURE);

	// create exe-elem "root"
	exe_stack = create_root_node();
	if (!exe_stack)
		return (FAILURE); // TODO:free
	stk_add_to_bottom(&info->exe_root, exe_stack);

	// create exec-elem "pipe" and create edge to root
	exe_stack = create_pipe_node();
	info->exe_root->next = exe_stack;

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
	debug_print_2d_arr(split_by_pipe, "split by pipe");

	i = 0;
	while (split_by_pipe[i])
	{
		exe_stack = create_pipe_elem(split_by_pipe[i]);
		if (!exe_stack)
			return (FAILURE); // TODO:free
	}


	// while (true)
	//  split &&, ||
	//  split space
	//  split |, ()

	return (SUCCESS);
}
