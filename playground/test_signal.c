/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 11:27:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/14 16:44:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

typedef void Sigfunc(int);
// signal -> Sigfunc *signal(int, Sigfunc *);

#if (defined(SIG_ING) && !defined(SIG_ERR))
#define SIG_ERR ((Sigfunc *)-1)
#endif

static void sig_handler(int signo); /* one handler for both signals */

int	count;



int main(void)
{
	count = 0;

	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if (signal(SIGUSR2, sig_handler) == SIG_ERR)
		printf("can't catch SIGUSR2\n");
	if (signal(SIGQUIT, sig_handler) == SIG_ERR)
		printf("can't catch SIGQUIT\n");
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("can't catch SIGINT\n");

	for (;;)
		pause();

	return (0);
}

static void sig_handler(int signo) /* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else if (signo == SIGQUIT)
		printf("received SIGQUIT\n");
	else if (signo == SIGINT)
	{
		printf("received SIGINT -> EXIT\n");
		exit(1);
	}
	else
		printf("received signal %d\n", signo);
	return ;
}




























