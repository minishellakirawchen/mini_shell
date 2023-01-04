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

// 2 cases, with -n or without -n
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

	i = 0;
	while (str && str[i])
	{
		ft_printf("%s", str[i++]);
		if (str[i])
			ft_printf(" ");
	}
	if (is_nl)
		ft_printf("\n");
}

int	ft_echo(t_info *info)
{
	char	*nl_flg;
	size_t	next_echo_chr_len;
	size_t	i;
	bool	is_nl;
	int		strcmp_res;

	if (!info || !info->commands || !info->commands[0])
		return (1); //TODO: exit?
	nl_flg = info->commands[1];
	next_echo_chr_len = ft_strlen_ns(nl_flg);
	i = 1;
	is_nl = true;
	strcmp_res = ft_strncmp_ns(nl_flg, "-n", next_echo_chr_len);
	if (next_echo_chr_len == 2 &&  strcmp_res == 0)
	{
		i++;
		is_nl = false;
	}
	print_echo(&info->commands[i], is_nl);
	return (0);
}
