/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 21:56:59 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtins(char *cmd_head)
{
	const size_t	len = ft_strlen_ns(cmd_head);

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

int	execute_builtins(char *cmd_head, t_info *info)
{
	const size_t	len = ft_strlen_ns(cmd_head);

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


// child  -> parent
// stdout -> stdin

int execute_pipe_recursion(t_stack *stk, t_info *info)
{

	pid_t		pid;
	int			pipe_fd[2];
	t_exe_elem	*elem;

	elem = NULL;
	if (stk->prev)
		elem = stk->prev->content;
	if (elem && elem->exe_type == E_CMD)
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid == 0) // child why 0?
		{
			close(pipe_fd[READ]);
			close(STDOUT_FILENO);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			execute_pipe_recursion(stk->prev, info);
		}

		close(pipe_fd[WRITE]);
		close(STDIN_FILENO);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	elem = stk->content;
	if (!elem || !elem->cmds)
		return (EXIT_FAILURE);
//	debug_print_2d_arr(elem->cmds, "cmds");
	/*
	if (is_builtins(elem->cmds[0]))
		info->exit_status = execute_builtins(elem->cmds[0], info);
	else
	{
//		execve(elem->cmds[0], elem->cmds, NULL);
		execvp(elem->cmds[0], elem->cmds);//TODO:exeve
		perror("execvp");
		printf("command not found: %s\n", elem->cmds[0]);
		info->exit_status = 127;
	}
	*/

	execvp(elem->cmds[0], elem->cmds);//TODO:exeve
	perror("execvp");
	printf("command not found: %s\n", elem->cmds[0]);
	exit (CMD_NOT_FOUND);
}

int	execute(t_info *info)
{
	pid_t	pid;
	t_stack	*pipe_last;
	size_t	i;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		pipe_last = get_last_elem(info->exe_root->next);
		execute_pipe_recursion(pipe_last, info);
	}
	// parent pid > 0; Process ID
	i = 0;
	while (i < get_stack_size(info->exe_root->next) + 1)
	{
		wait(&status);
		i++;
	}
	info->exit_status = WEXITSTATUS(status);
//	info->exit_status = waitpid(pid, NULL, 0);
	return (info->exit_status);
}
