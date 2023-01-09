/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 18:28:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 20:03:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*get_last_node(t_tree *elem)
{
	if (!elem)
		return (NULL);
	while (elem->next)
		elem = elem->next;
	return (elem);
}

void	tree_clear(t_tree **stk)
{
	t_tree	*right;

	if (!stk)
		return ;
	while (*stk)
	{
		right = (*stk)->next;
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
		stk = stk->next;
		size++;
	}
	return (size);
}
