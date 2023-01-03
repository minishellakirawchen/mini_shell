/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 08:23:01 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"
#include "./../includes/excution.h"
#include "./../includes/exit.h"

int	execute(t_info *info)
{
	pid_t	pid;
	char 	**input_arg = ft_split_set(info->input_line, ' ', '\'');//tmp

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free_and_return_no(&info, EXIT_FAILURE);
	}
	if (pid == 0) // child
	{
		execvp(input_arg[0], input_arg);// tmp test.c.c.c

		// if cmd has path -> execve(PATH, cmd, env);
		// else            -> ft_execvp(file, cmd); search path and execve
		free_and_return_no(&info, EXIT_FAILURE);
	}
	// parent
	info->exit_status = waitpid(pid, NULL, 0);
	return (info->exit_status);
}
