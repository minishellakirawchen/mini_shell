/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 11:27:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 17:17:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef void Sigfunc(int);
// signal -> Sigfunc *signal(int, Sigfunc *);

#if (defined(SIG_ING) && !defined(SIG_ERR))
#define SIG_ERR ((Sigfunc *)-1)
#endif

int sigint_cnt = 0;
int sig_flg = 0;
volatile sig_atomic_t	g_flag = 0;

void sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
//		printf("\n"); // Move to NL
//		rl_on_new_line();
//		rl_replace_line("", 0);
//		rl_redisplay();
		sigint_cnt++;
		printf("sigint_cnt:%d\n", sigint_cnt);
	}
	if (signo == SIGQUIT)
		printf("^\\Quit\n");
}

Sigfunc *signal_act(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}

int main(void)
{
	struct sigaction sa_sigint;
	char *line;

	memset(&sa_sigint, 0, sizeof(sa_sigint));
	// signal hook
	sa_sigint.sa_handler = sigint_handler;
	sa_sigint.sa_flags = 0;

	if (signal_act(SIGINT, sigint_handler) == SIG_ERR)
		perror("sigaction");

	if (signal_act(SIGQUIT, sigint_handler) == SIG_ERR)
		perror("sigaction");

	sigint_cnt = 0;
	printf("1. sigint_cnt:%d\n", sigint_cnt);
	while (sigint_cnt <= 10)
	{
		/* pause中だとquitが有効になる */
		/*
		printf("sleep start\n");
		sleep(10);
		printf("sleep end\n");
		pause();
		*/

		/* 入力待ちだとquitが効かない */
		line = readline("input $> ");
		if (!line)
		{
			dprintf(STDERR_FILENO, "^D Pressed\n");
			break ;
		}
	}
	printf("2. sigint_cnt:%d\n", sigint_cnt);
	return (0);
}





/*
gcc test_signal.c -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline && ./a.out
*/
