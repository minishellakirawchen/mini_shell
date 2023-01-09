/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:58:03 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 15:04:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_tree_node(t_exe_type type, char ***cmds)
{
	t_tree		*new_node;

	new_node = (t_tree *)malloc(sizeof(t_tree));
	if (!new_node)
		return (perror_and_ret_nullptr("malloc"));
	new_node->exe_type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->redirect_info = NULL;
	if (!cmds)
	{
		new_node->cmds = NULL;
		return (new_node);
	}
	new_node->cmds = *cmds; //TODO:copyされる？ freeはどこで？
	new_node->redirect_info = get_redirection_info(new_node->cmds);
	if (!new_node->redirect_info)
	{
		free_1d_array_ret_nullptr((void **)&new_node);
		return (NULL);
	}
	return (new_node);
}
