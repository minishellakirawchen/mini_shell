/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 18:28:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 09:15:52 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_tree_elem(void *content)
{
	t_tree	*new_elem;

	new_elem = (t_tree *)malloc(sizeof(t_tree));
	if (!new_elem)
		return (NULL);
	new_elem->content = content;
	new_elem->left = NULL;
	new_elem->right = NULL;
	return (new_elem);
}

t_tree	*get_last_elem(t_tree *elem)
{
	if (!elem)
		return (NULL);
	while (elem->right)
		elem = elem->right;
	return (elem);
}

void	tree_clear(t_tree **stk)
{
	t_tree	*right;

	if (!stk)
		return ;
	while (*stk)
	{
		right = (*stk)->right;
		free(*stk);
		*stk = right;
	}
	*stk = NULL;
}

size_t	get_tree_size(t_tree *stk)
{
	size_t	size;

	size = 0;
	while (stk)
	{
		stk = stk->right;
		size++;
	}
	return (size);
}
