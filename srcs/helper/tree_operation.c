/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:19:29 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 13:02:24 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_top_of_tree(t_tree **root, t_tree *elem)
{
	if (!root)
		return ;
	elem->prev = NULL;
	elem->next = *root;
	if (*root)
		(*root)->prev = elem;
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
		elem->next = NULL;
		elem->prev = NULL;
		return ;
	}
	bottom_elem = get_last_node(*root);
	bottom_elem->next = elem;
	elem->prev = bottom_elem;
	elem->next = NULL;
}

t_tree	*pop_from_top(t_tree **root)
{
	t_tree	*top_elem;

	if (!root || !*root)
		return (NULL);
	top_elem = *root;
	*root = top_elem->next;
	if (top_elem->next)
		top_elem->next->prev = NULL;
	top_elem->prev = NULL;
	top_elem->next = NULL;
	return (top_elem);
}

t_tree	*pop_from_bottom(t_tree **tree)
{
	t_tree	*bottom_elem;

	if (!tree || !*tree)
		return (NULL);
	bottom_elem = get_last_node(*tree);
	if (bottom_elem->prev)
		bottom_elem->prev->next = NULL;
	bottom_elem->prev = NULL;
	bottom_elem->next = NULL;
	return (bottom_elem);
}
