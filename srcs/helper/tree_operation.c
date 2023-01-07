/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:19:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 09:14:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_top_of_tree(t_tree **root, t_tree *elem)
{
	if (!root)
		return ;
	elem->left = NULL;
	elem->right = *root;
	if (*root)
		(*root)->left = elem;
	*root = elem;
}

void	add_bottom_of_tree(t_tree **root, t_tree *elem)
{
	t_tree	*bottom_elem;

	if (!root)
		return ;
	if (!*root)
	{
		*root = elem;
		elem->right = NULL;
		elem->left = NULL;
		return ;
	}
	bottom_elem = get_last_elem(*root);
	bottom_elem->right = elem;
	elem->left = bottom_elem;
	elem->right = NULL;
}

t_tree	*pop_from_top(t_tree **root)
{
	t_tree	*top_elem;

	if (!root || !*root)
		return (NULL);
	top_elem = *root;
	*root = top_elem->right;
	if (top_elem->right)
		top_elem->right->left = NULL;
	top_elem->left = NULL;
	top_elem->right = NULL;
	return (top_elem);
}

t_tree	*pop_from_bottom(t_tree **tree)
{
	t_tree	*bottom_elem;

	if (!tree || !*tree)
		return (NULL);
	bottom_elem = get_last_elem(*tree);
	if (bottom_elem->left)
		bottom_elem->left->right = NULL;
	bottom_elem->left = NULL;
	bottom_elem->right = NULL;
	return (bottom_elem);
}
