/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 13:47:19 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 14:57:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtins(char *cmd_head)
{
	const size_t	len = ft_strlen_ns(cmd_head);

	if (len == 0)
		return (false);
	// TODO: implement more simple
	//     : change ft_** to **
	if (ft_strncmp_ns("ft_echo", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_ECHO", cmd_head, len) == 0))
		return (true);
	if (ft_strncmp_ns("ft_cd", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_pwd", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_PWD", cmd_head, len) == 0))
		return (true);
	if (ft_strncmp_ns("ft_export", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_unset", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_env", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_ENV", cmd_head, len) == 0))
		return (true);
	if (ft_strncmp_ns("ft_exit", cmd_head, len) == 0)
		return (true);
	return (false);
}

int	execute_builtins(t_info *info, char **cmds)
{
	const char		*cmd_head = cmds[0];
	const size_t	len = ft_strlen_ns(cmd_head);

	// TODO: implement more simple
	//     : change ft_** to **
	if (ft_strncmp_ns("ft_echo", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_ECHO", cmd_head, len) == 0))
		return (ft_echo(info, cmds));
	if (ft_strncmp_ns("ft_cd", cmd_head, len) == 0)
		return (ft_cd(info, cmds));
	if (ft_strncmp_ns("ft_pwd", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_PWD", cmd_head, len) == 0))
		return (ft_pwd(info));
	if (ft_strncmp_ns("ft_export", cmd_head, len) == 0)
		return (ft_export(info, cmds));
	if (ft_strncmp_ns("ft_unset", cmd_head, len) == 0)
		return (ft_unset(info, cmds));
	if (ft_strncmp_ns("ft_env", cmd_head, len) == 0 \
	|| (ft_strncmp_ns("ft_ENV", cmd_head, len) == 0))
		return (ft_env(info, cmds));
	if (ft_strncmp_ns("ft_exit", cmd_head, len) == 0)
		return (ft_exit(info, cmds));
	return (EXIT_FAILURE);
}
