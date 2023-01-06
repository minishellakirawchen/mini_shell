/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:12:53 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 13:34:51 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// [echo hello wold "a b c hoge "]	-> {echo, hello, world, "a b c hoge ", NULL};
// [cat Makefile | grep a]			-> {cat, Makefile, |, grep, a}

// 先にsplitせず、まず|でsplitしたものをexec_stackに入れていく

t_stack	*create_root(void)
{
	t_exe_stk	*exe_elem;
	t_stack		*root;

	exe_elem = (t_exe_stk *)malloc(sizeof(t_exe_stk));
	if (!exe_elem)
	{
		perror("malloc");
		return (NULL);
	}
	exe_elem->exe_type = E_ROOT;
	exe_elem->cmds = NULL;
	root = create_stack_elem(exe_elem);
	if (!root)
	{
		perror("malloc");
		return (NULL);
	}
	return (root);
}

int	analysis(t_info *info)
{
//	t_exe_stk	*exe_stk;
	char 		**split_char;

//	exe_stk = NULL;
	if (!info)
		return (false);
	info->exe_root = create_root();
	if (!info->exe_root)
		return (false);
//	split_char = ft_split(info->input_line, '|');//""は残す？
	split_char = ft_split_set(info->input_line, '|', '"');
	debug_print_2d_arr(split_char, "split by pipe");

	// split space
	// split &&, ||
	// split |, ()

	return (true);
}
