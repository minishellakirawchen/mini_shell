/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:27:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	ft_printf("%s=%s\n", elem->key, elem->value);
}

// TODO: $> env CMD
//   ex) $> env ./minishell -> minishell run use env
// env -op -> error
int ft_env(t_info *info, const char **cmds)
{
	if (!info || !cmds)
		return (1); //TODO: exit?
	ft_lstiter(info->env_list, print_key_value);
	if (cmds[1])
		ft_dprintf(STDERR_FILENO, "minishell: env : Unknown options");
	return (0);
}
