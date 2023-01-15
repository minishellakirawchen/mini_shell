/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 11:16:42 by takira           ###   ########.fr       */
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

void sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n"); // Move to NL
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

sigfunc	*signal_act(int signo, sigfunc *func)
{
	struct sigaction	act;
	struct sigaction	oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;
	struct	sigaction sa_sigint;

	ft_memset(&sa_sigint, 0, sizeof(sa_sigint));
	sa_sigint.sa_handler = sigint_handler;
	sa_sigint.sa_flags = 0;

	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		perror("sigaction");

	while (true)
	{
		input_line = readline(PROMPT);
		if (!input_line)
		{
			exit_status = EXIT_SUCCESS;
			ft_printf("exit");
			break ;
		}
		//TODO: is necessary clear history?
		printf("input            :[%s]\n", input_line);
		analysis(info, input_line);//TODO cmd op重複も解析、削除するか？
		expansion(info);
//		printf("[#DEGUG] vvvvv Execution vvvvv\n"); // tmp
		exit_status = execute_command_line(info);
		info->exit_status = exit_status;
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
