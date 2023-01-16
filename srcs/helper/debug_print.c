/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/12 12:45:18 by takira           ###   ########.fr       */
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
		dprintf(STDERR_FILENO, "%s:", str);
	dprintf(STDERR_FILENO, "{");
	while (arr[i])
	{
		dprintf(STDERR_FILENO, "\"%s\"", arr[i++]);
		if (arr[i])
			dprintf(STDERR_FILENO, ", ");
	}
	dprintf(STDERR_FILENO, "}\n");
}

void	debug_print_stack(t_tree *root, char *str)
{
	t_tree		*tree;

	dprintf(STDERR_FILENO, "\n");
	if (str)
		dprintf(STDERR_FILENO, "#DEBUG[print_stack : %s]\n", str);
	tree = root;
	while (tree)
	{
		if (tree->exe_type == E_NODE_ROOT)
		{
			dprintf(STDERR_FILENO, " [root]\n");
			dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_NODE_PIPE)
		{
			dprintf(STDERR_FILENO, " [pipe]\n");
			dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_NODE_NO_PIPE)
		{
			dprintf(STDERR_FILENO, " [shell]\n");
			dprintf(STDERR_FILENO, "  |    \n");
		}
		else if (tree->exe_type == E_LEAF_COMMAND)
		{
			dprintf(STDERR_FILENO, " [cmd]--");
			debug_print_2d_arr(tree->cmds, NULL);
		}
		tree = tree->next;
	}
	dprintf(STDERR_FILENO, "\n");
}

