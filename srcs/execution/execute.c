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

bool	is_builtins(t_info *info)
{
	char	*cmd_head;
	size_t	len;

	if (!info || !info->commands)
		return (false);
	cmd_head = info->commands[0];
	len = ft_strlen_ns(cmd_head);
//	printf("#[DEBUG]commands[0]:%s, len:%zu\n", cmd_head, len);
	if (len == 0)
		return (false);
	// TODO: implement more simple, for debug, ft_**
	if (ft_strncmp_ns("ft_echo", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_ECHO", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_cd", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_pwd", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_PWD", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_export", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_unset", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_env", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_ENV", cmd_head, len) == 0)
		return (true);
	if (ft_strncmp_ns("ft_exit", cmd_head, len) == 0)
		return (true);
	return (false);
}

int	execute_builtins(t_info *info)
{
	char	*cmd_head;
	size_t	len;

	if (!info || !info->commands)
		return (EXIT_FAILURE);
	cmd_head = info->commands[0];
	len = ft_strlen_ns(cmd_head);
	// TODO:  implement more simple
	if (ft_strncmp_ns("ft_echo", cmd_head, len) == 0)
		return (ft_echo(info));
	if (ft_strncmp_ns("ft_ECHO", cmd_head, len) == 0)
		return (ft_echo(info));
	if (ft_strncmp_ns("ft_cd", cmd_head, len) == 0)
		return (ft_cd(info));
	if (ft_strncmp_ns("ft_pwd", cmd_head, len) == 0)
		return (ft_pwd(info));
	if (ft_strncmp_ns("ft_PWD", cmd_head, len) == 0)
		return (ft_pwd(info));
	if (ft_strncmp_ns("ft_export", cmd_head, len) == 0)
		return (ft_export(info));
	if (ft_strncmp_ns("ft_unset", cmd_head, len) == 0)
		return (ft_unset(info));
	if (ft_strncmp_ns("ft_env", cmd_head, len) == 0)
		return (ft_env(info));
	if (ft_strncmp_ns("ft_ENV", cmd_head, len) == 0)
		return (ft_env(info));
	if (ft_strncmp_ns("ft_exit", cmd_head, len) == 0)
		return (ft_exit(info));
	return (EXIT_FAILURE);
}

//** builtins **
//  cmd		: char **commands (readline->input func->expand_vars)
//  echo or ECHO
// 		{"echo", "-n", "foo", NULL}
// 		{"echo", "-nnnnn", "foo", NULL} -> {"echo", "-n", "foo", NULL} re-shape in analysis
//		{"echo", "-n", NULL}
//		{"echo", "bar", NULL}
//		{"echo", NULL}
//		{"echo", "-n", "$foo", NULL} -> {"echo", "-n", "bar", NULL} expand $foo=bar in analysis
//		{"echo", "$foo", NULL} -> {"echo", "bar", NULL} expand $foo=bar in analysis
//
//  cd
//  	{"cd", "relative path", NULL}
//		{"cd", "absolute path", NULL}
//
//  pwd or PWD
// 		{"pwd", "NULL"}
//
//  export
//  	{"export", "NULL"}
//		{"export", $foo, NULL} -> {"export", bar, NULL} expand $foo=bar in analysis
//
//  unset
//  	{"unset", NULL}
//
//  env or ENV
//  	{"env", NULL}
//
//	exit
//		{"exit", NULL}

int	execute(t_info *info)
{
	pid_t	pid;

	if (is_builtins(info))
		return (execute_builtins(info));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free_and_return_no(&info, EXIT_FAILURE);
	}
	if (pid == 0) // child why 0?
	{
		execvp(info->input_line[0], info->input_line);// for tmp test

		// if cmd has path -> execve(PATH, cmd, env);
		// else            -> ft_execvp(file, cmd); search path and execve
		perror("execvp");
		free_and_return_no(&info, EXIT_FAILURE);
	}
	// parent pid > 0; Process ID
	info->exit_status = waitpid(pid, NULL, 0);
	return (info->exit_status);
}
