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
//
// idx0  1   2  ..
// echo -n  XXX YYY ..
// echo XXX YYY ..
//
// $>echo hello world -> $> hello world
//
// $xxx->expand by analysis and expansion part
// echo just print it

static void	print_echo(char **str, bool is_nl)
{
	size_t	i;

	if (!str)
		return ;
	i = 0;
	while (str && str[i])
		ft_printf("%s", str[i++]);
	if (is_nl)
		ft_printf("\n");
}

int	ft_echo(t_info *info)
{
	const char		*nl_flg = info->commands[1];
	const size_t	len = ft_strlen_ns(nl_flg);
	size_t			i;
	bool			is_nl;

	i = 1;
	is_nl = true;
	if (len == 2 && ft_strncmp_ns(nl_flg, "-n", len) == 0)
	{
		i++;
		is_nl = false;
	}
	print_echo(&info->commands[i], is_nl);
	return (0);
}