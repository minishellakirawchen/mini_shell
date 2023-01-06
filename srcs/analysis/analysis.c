/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 19:36:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まずpipeでsplitしたものをexec_stackに入れていく
// その際にsplie(space), trim(space)して整形する
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
		free(exe_elem);
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
		free(exe_elem);
		return (NULL);
	}
	return (pipe_node);
}

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
	debug_print_2d_arr(splitted_strs, "splited_str[i]");
	return (splitted_strs);
}

t_stack	*create_cmd_elem(char *cmd)
{
	t_exe_elem	*exe_elem;
	t_stack		*root;

	exe_elem = (t_exe_elem *)malloc(sizeof(t_exe_elem));
	if (!exe_elem)
	{
		perror("malloc");
		return (NULL);
	}
	exe_elem->exe_type = E_CMD;
	exe_elem->cmds = splitset_and_trim(cmd, ' ', '"', ISSPACE);
	if (!exe_elem->cmds)
	{
		perror("malloc");
		free(exe_elem);
		return (NULL);
	}
	root = create_stack_elem(exe_elem);
	if (!root)
	{
		perror("malloc");
		free(exe_elem);
		return (NULL);
	}
	return (root);
}

// First try, create commands which connected 1 level pipe, like: $> cat Makefile | grep a | grep b
int	analysis(t_info *info)
{
	t_stack		*exe_stack;
	t_stack		*prev;
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
	prev = info->exe_root->next;
	while (split_by_pipe[i])
	{
		exe_stack = create_cmd_elem(split_by_pipe[i]);
		if (!exe_stack)
			return (FAILURE); // TODO:free
		prev->next = exe_stack;
		prev = exe_stack;
		i++;
	}

	// while (true)
	//  split &&, ||
	//  split space
	//  split |, ()

	return (SUCCESS);
}
