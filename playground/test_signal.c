/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 11:27:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/14 12:45:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

typedef void Sigfunc(int);
// signal -> Sigfunc *signal(int, Sigfunc *);

#if (defined(SIG_ING) && !defined(SIG_ERR))
#define SIG_ERR ((Sigfunc *)-1)
#endif

static void sig_usr(int); /* one handler for both signals */

int main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR2\n");

	for (;;)
		pause();

	return (0);
}

static void sig_usr(int signo) /* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		printf("received signal %d\n", signo);
	return ;
}