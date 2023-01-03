/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 13:56:37 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 13:56:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 2 cases, with -n or with out -n
// idx0  1  2  ...
// echo -n XXX ,   NULL
// echo XXX
//
// $>echo hello world -> $> hello world
//
// echo $xxx->expand by expansion part
// echo just print it

//TODO: implement more simple
int	ft_echo(t_info *info)
{
	const char		*nl_flg = info->commands[1];
	const size_t	len = ft_strlen_ns(nl_flg);
	size_t			i;

	i = 1;
	if (len == 2 && ft_strncmp_ns(nl_flg, "-n", len) == 0)
	{
		i++;
		while (info->commands[i])
		{
			ft_printf("%s", info->commands[i++]);
			if (info->commands[i])
				ft_printf(" ");
		}
	}
	else
	{
		while (info->commands[i])
		{
			ft_printf("%s", info->commands[i++]);
			if (info->commands[i])
				ft_printf(" ");
		}
		ft_printf("\n");
	}
	return (0);
}