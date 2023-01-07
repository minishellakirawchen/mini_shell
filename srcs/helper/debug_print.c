/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:21:33 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 10:01:49 by takira           ###   ########.fr       */
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
	t_tree		*ptr;
	t_exe_elem	*elem;

	ft_printf("\n");
	if (str)
		ft_printf("#DEBUG[print_stack : %s]\n", str);
	ptr = root;
	while (ptr)
	{
		elem = ptr->content;
		if (elem->exe_type == E_ROOT)
		{
			ft_printf(" [root]\n");
			ft_printf("  |    \n");
		}
		else if (elem->exe_type == E_PIPE)
		{
			ft_printf(" [pipe]\n");
			ft_printf("  |    \n");
		}
		else if (elem->exe_type == E_CMD)
		{
			ft_printf(" [cmd]--");
			debug_print_2d_arr(elem->cmds, NULL);
		}
		ptr = ptr->right;
	}
	ft_printf("\n");
}

