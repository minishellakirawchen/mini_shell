/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 09:02:32 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int sig_flg;

void	sig_handler_for_prompt_loop(int signo)
{
	if (signo == SIGINT)
	{
		sig_flg = SIGINT;
		printf("SIGINT: test newline, flg:%d\n", sig_flg);
	}
	if (signo == SIGQUIT)
	{
		sig_flg = SIGQUIT;
		printf("SIGQUIT: test quit, flg:%d\n", sig_flg);
	}
}

void	init_input(t_info **info)
{
	if (!info || !*info)
		return ;
	free_2d_array_ret_nullptr((void ***)&(*info)->splitted_cmds);
	tree_clear(&(*info)->tree_root);
	(*info)->splitted_cmds = NULL;
	(*info)->tree_root = NULL;
}

void sig_handle(void)
{
	sig_flg = 0;
	if (signal(SIGINT, sig_handler_for_prompt_loop) == SIG_ERR)
		perror("signal");
	if (signal(SIGQUIT, sig_handler_for_prompt_loop) == SIG_ERR)
		perror("signal");
}

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;

	while (true)
	{
//		sig_handle();
		input_line = readline(PROMPT);
		if (sig_flg == SIGINT)
		{
//			rl_on_new_line();
			continue ;
		}
		sig_flg = 0;
		if (!input_line)
			break ;

		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		// if (input signal == ^C)
		//		print "^C" and not add history
		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/* Maybe unnecessary, "clear" clear prompt on default */
		// if (strncmp("clear", inpuf_line, ft_strlen("clear")) == 0)
		//		rewrite prompt
		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		//TODO: is necessary clear history?
		printf("input            :[%s]\n", input_line);
		analysis(info, input_line);//TODO cmd op重複も解析、削除するか？
		expansion(info);
//		arrange_command_line(info);
//		printf("[#DEGUG] vvvvv Execution vvvvv\n"); // tmp
		exit_status = execute_command_line(info);
		info->exit_status = exit_status;
//		printf("[#DEGUG] ^^^^^ Execution ^^^^^\n"); // tmp
//		printf("[#DEGUG] ^^^^^ Execution ^^^^^\n"); // tmp
		dprintf(STDERR_FILENO, "[#DEBUG]exit status:%d\n", info->exit_status);
		add_history(input_line);//tmp
		free(input_line);
		if (info->is_exit)
			break ;
		init_input(&info);
	}
	return (exit_status);
}
