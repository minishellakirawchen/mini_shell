/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 13:12:24 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_info **info)
{
	if (!info || !*info)
		return ;
	free_2d_array_ret_nullptr((void ***)&(*info)->splitted_cmds);
	tree_clear(&(*info)->tree_root);
	(*info)->splitted_cmds = NULL;
	(*info)->tree_root = NULL;
}

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;

	errno = 0;
	init_signal_in_prompt();
	while (true)
	{
		input_line = readline(PROMPT);
		if (!input_line)
		{
			exit_status = EXIT_SUCCESS;
			ft_dprintf(STDERR_FILENO, "exit\n");
			break ;
		}
		add_history(input_line);
//		printf("input            :[%s]\n", input_line);
		analyze_input(info, input_line);
		expansion(info);
//		printf("[#DEGUG] vvvvv Execution vvvvv\n"); // tmp
		exit_status = execute_command_line(info);
		info->exit_status = exit_status;
//		printf("[#DEGUG] ^^^^^ Execution ^^^^^\n"); // tmp
//		dprintf(STDERR_FILENO, "[#DEBUG]exit status:%d\n", info->exit_status);
		free_1d_array_ret_nullptr((void **)&input_line);
		if (info->is_exit)
			break ;
		init_input(&info);
	}
	return (exit_status);
}
