/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 21:48:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: input->handling
// ex) exit 1   -> echo $? = 1
// ex) exit 255 -> echo $? = 255
// ex) exit 256 -> echo $? = 1
// ex) exit a   -> error

// bash $> exit a b
// exit
// bash: exit: a: numeric argument required
// zsh  $> echo $?
// 255

// bash $> exit 1 a
// exit
// bash: exit: too many arguments
// bash $> echo $?
// 1

// bash $> exit 1 1
// exit
// bash: exit: too many arguments
// bash $> echo $?
// 1

// cmds = {exit, foo, bar, .., NULL}

// exit 2147483647 -> 255
// exit 2147483648 -> 0

// exit 9223372036854775807 -> 255
// exit 9223372036854775808 -> bash: exit: 9223372036854775808: numeric argument required

int	ft_exit(t_info *info, const char **cmds)
{
	int				exit_status;
	bool			is_strtoll_success;
	const size_t	argc = get_2d_array_size(cmds);

	if (!info)
		return (FAILURE);
	ft_dprintf(STDERR_FILENO, "exit");
	if (argc > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments");
		return (EXIT_TOO_MANY_ARGS);
	}
	exit_status = EXIT_SUCCESS;
	if (argc == 2)
	{
		exit_status = (int)(ft_strtoll((char *)cmds[1], &is_strtoll_success) % 256);
		if (!is_strtoll_success)
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required", (char *)cmds[1]);
			exit (EXIT_NUMERIC_ARGS_REQUIRED);
		}
	}
	info->is_exit = true;
	exit_status %= 256;
	exit(exit_status);
}
