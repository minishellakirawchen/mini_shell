/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 13:25:34 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void debug_print_2d_arr(char **arr, char *str)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	if (str)
		ft_printf("%s:", str);
	ft_printf("{");
	while (arr[i])
	{
		ft_printf("\"%s\"", arr[i++]);
		if (arr[i])
			ft_printf(", ");
	}
	ft_printf("}\n");
}

void	debug_print_stack(t_tree *root, char *str)
{
	t_tree		*tree;

	ft_printf("\n");
	if (str)
		ft_printf("#DEBUG[print_stack : %s]\n", str);
	tree = root;
	while (tree)
	{
		if (tree->exe_type == E_NODE_ROOT)
		{
			ft_printf(" [root]\n");
			ft_printf("  |    \n");
		}
		else if (tree->exe_type == E_NODE_PIPE)
		{
			ft_printf(" [pipe]\n");
			ft_printf("  |    \n");
		}
		else if (tree->exe_type == E_NODE_NO_PIPE)
		{
			ft_printf(" [shell]\n");
			ft_printf("  |    \n");
		}
		else if (tree->exe_type == E_LEAF_COMMAND)
		{
			ft_printf(" [cmd]--");
			debug_print_2d_arr(tree->cmds, NULL);
		}
		tree = tree->next;
	}
	ft_printf("\n");
}

