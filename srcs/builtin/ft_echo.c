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

// echo -n XXX
// echo XXX
// echo $xxx->expand by expansion part
// echo just print it

//
int	ft_echo(t_info *info)
{
	int				exit_status;
	const char		*nl_flg = info->commands[1];
	const size_t	len = ft_strlen_ns(nl_flg);

	if (len == 2 && ft_strncmp_ns(nl_flg, "-n", len) == 0)
	{
		if (ft_strlen_ns(info->commands[2]))
			ft_printf("ft_echo[%s]", info->commands[2]);
		else
			ft_printf("ft_echo[]");
	}
	else
	{
		if (len)
			ft_printf("ft_echo[%s]\n", info->commands[1]);
		else
			ft_printf("ft_echo[]\n");
	}
	exit_status = EXIT_SUCCESS;
	return (exit_status);
}