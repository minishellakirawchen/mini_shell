/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:52:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 17:08:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void signal_handler_in_prompt(int signo)
{
	if (signo == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void signal_handler_in_execution(int signo)
{
	if (signo == SIGQUIT)
	{
		ft_printf("Quit: 3\n");
		//TODO: info->global
		// kill(SIGINT) to all child
	}
}
