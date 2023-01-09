/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 19:08:15 by takira           ###   ########.fr       */
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
int ft_exit(t_info *info, const char **cmds)
{
	int		exit_status;
	bool	is_atoi_success;
	size_t	argc = get_2d_array_size(cmds);

	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_TOO_MANY_ARGS);
	}
	exit_status = EXIT_SUCCESS;
	if (info)
		exit_status = info->exit_status;
	if (argc == 2)
	{
		exit_status = ft_atoi(cmds[1], &is_atoi_success);
		if (!is_atoi_success)
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd((char *)cmds[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit (EXIT_NUMERIC_ARGS_REQUIRED);
		}
		exit_status %= 255;
	}
	free_info(&info);
	exit_status = 10;
	exit(exit_status);
}
