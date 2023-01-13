/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 13:47:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 18:58:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: change ft_XX -> XX
bool	is_builtins(const char **cmds)
{
	if (is_same_str("ft_echo",  cmds[0]) || is_same_str("ft_ECHO",  cmds[0]))
		return (true);
	if (is_same_str("ft_cd",  cmds[0]))
		return (true);
	if (is_same_str("ft_pwd",  cmds[0]) || is_same_str("ft_PWD",  cmds[0]))
		return (true);
	if (is_same_str("ft_export",  cmds[0]))
		return (true);
	if (is_same_str("ft_unset",  cmds[0]))
		return (true);
	if (is_same_str("ft_env",  cmds[0]) || is_same_str("ft_ENV",  cmds[0]))
		return (true);
	if (is_same_str("ft_exit",  cmds[0]))
		return (true);
	return (false);
}

// TODO: change ft_XX -> XX
int	execute_builtin(t_info *info, const char **cmds)
{
	if (is_same_str("ft_echo",  cmds[0]) || is_same_str("ft_ECHO",  cmds[0]))
		return (ft_echo(info, cmds));
	if (is_same_str("ft_cd",  cmds[0]))
		return (ft_cd(info, cmds));
	if (is_same_str("ft_pwd",  cmds[0]) || is_same_str("ft_PWD",  cmds[0]))
		return (ft_pwd(info));
	if (is_same_str("ft_export",  cmds[0]))
		return (ft_export(info, cmds));
	if (is_same_str("ft_unset",  cmds[0]))
		return (ft_unset(info, cmds));
	if (is_same_str("ft_env",  cmds[0]) || is_same_str("ft_ENV",  cmds[0]))
		return (ft_env(info, cmds));
	if (is_same_str("ft_exit",  cmds[0]))
		return (ft_exit(info, cmds));
	return (EXIT_FAILURE);
}
