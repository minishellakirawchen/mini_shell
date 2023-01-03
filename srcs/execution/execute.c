/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 10:50:04 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// minishell> ls -l
//            {"ls", " -l", NULL}

bool	is_builin(t_info *info)
{

}

int	execute_builtin(t_info *info)
{

}

// builtin
//   ehoo: {"echo", "-n", NULL}

int	execute(t_info *info)
{
	pid_t	pid;

	if (is_builin(info))
		return (execute_builtin(info));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free_and_return_no(&info, EXIT_FAILURE);
	}
	if (pid == 0) // child why 0?
	{
		execvp(info->input_line[0], info->input_line);// tmp test

		// if cmd has path -> execve(PATH, cmd, env);
		// else            -> ft_execvp(file, cmd); search path and execve
		perror("execvp");
		free_and_return_no(&info, EXIT_FAILURE);
	}
	// parent pid > 0
	info->exit_status = waitpid(pid, NULL, 0);
	return (info->exit_status);
}
