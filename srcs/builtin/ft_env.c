/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 20:31:19 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
//	printf(" key[%s], value:[%s]\n", elem->key, elem->value);
	printf("%s=%s\n", elem->key, elem->value);
}

int ft_env(t_info *info)
{
	if (!info)
		return (1);
	ft_lstiter(info->env_list, print_key_value);
	return (0);
}
