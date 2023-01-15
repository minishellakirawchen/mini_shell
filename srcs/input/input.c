/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:51:15 by takira           ###   ########.fr       */
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

void signal_in_prompt(void)
{
	struct	sigaction	sigaction_sigint;
	struct	sigaction	sigaction_sigquit;

	errno = 0;
	ft_memset(&sigaction_sigint, 0, sizeof(sigaction_sigint));
	sigaction_sigint.sa_handler = signal_handler_in_prompt;
	sigaction_sigint.sa_flags = 0;
	ft_memset(&sigaction_sigquit, 0, sizeof(sigaction_sigquit));
	sigaction_sigquit.sa_handler = SIG_IGN;
	sigaction_sigquit.sa_flags = 0;
	if (signal_act(SIGINT, signal_handler_in_prompt) == SIG_ERR)
		perror("sigaction");
	if (signal_act(SIGQUIT, signal_handler_in_prompt) == SIG_ERR)
		perror("sigaction");
}

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;

	errno = 0;
	signal_in_prompt();
	while (true)
	{
		input_line = readline(PROMPT);
		if (!input_line)
		{
			exit_status = EXIT_SUCCESS;
			ft_printf("exit\n");
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
