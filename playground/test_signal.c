/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 11:27:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 10:20:36 by takira           ###   ########.fr       */
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

void signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n"); // Move to NL
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		sigint_cnt++;
	}
}

void signal_action(int signo, siginfo_t *info, void *ctx)
{
	if (signo == SIGINT)
	{
		printf("\n"); // Move to NL
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		sigint_cnt++;
	}
}

int main(void)
{
	struct sigaction sa_sigint;
	char *line;

	memset(&sa_sigint, 0, sizeof(sa_sigint));
	// signal hook
	sa_sigint.sa_handler = signal_handler;//ignore SIG_IGN;

	sa_sigint.sa_flags = 0;

	if (sigaction(SIGQUIT, &sa_sigint, NULL) < 0)
		perror("sigaction");
	if (sigaction(SIGINT, &sa_sigint, NULL) < 0)
		perror("sigaction");

	printf("1. sigint_cnt:%d\n", sigint_cnt);
	while (true)
	{
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
