/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 21:34:43 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 08:56:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

typedef enum e_group
{
	e_pipe,
	e_subshell,
	e_and,
	e_or,
}	t_group;

typedef struct s_tree
{
	t_group			group;
	char			**commands;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

int main(void)
{
 `

	return (0);
}