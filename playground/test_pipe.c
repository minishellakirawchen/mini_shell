/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 10:28:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/13 10:49:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void)
{
	/* cat | ls, case of fork->fork */
	//https://zariganitosh.hatenablog.jp/entry/20130722/process_pape_redirect_file_descriptor

	pid_t	pid1, pid2;
	int		pipe_fd[2];
	char	*c_cat[10] = {"cat", NULL};
	char	*c_ls[10] = {"ls", NULL};

	pipe(pipe_fd);
	pid1 = fork();
	if (pid1 == 0)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);

		dprintf(STDERR_FILENO, " 2. child process : exec cat\n");
		execvp(c_cat[0], c_cat);
		perror("execvp");
		exit (1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);

		dprintf(STDERR_FILENO, " 2. child process : exec ls\n");
		execvp(c_ls[0], c_ls);
		perror("execvp");
		exit (2);
	}
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
//	wait(NULL);
//	wait(NULL);

	dprintf(STDERR_FILENO, "1-1. main process\n");
	// wait どうする？
	int status1;
	waitpid(pid1, &status1, 0);
	wait(NULL);
//	while (wait(NULL) >= 0);
//	while (waitpid(-1, NULL, 0));

	dprintf(STDERR_FILENO, "1-2. main process fin\n");
	dprintf(STDERR_FILENO, "       status1:%d\n", WEXITSTATUS(status1));


	/* [main]fork ->[child] fork ->[grand child] 23/Jan/13th */
	/* how working exit child while working grand child? */
	// pid1, pid2をwaitしなければmainが先に終了してしまう
	// granc child終了後は待機状態となり、何かを入力するとコンソールに戻る <- cat | lsみたい？
	/*
	pid_t pid1, pid2;

	printf("## start ##\n");
	pid1 = fork();
	if (pid1 == 0)
	{
		pid2 = fork();
		if (pid2 == 0)
		{
			int i = 0;
			printf("  3-1. grand child process\n");
			while (true)
			{
				sleep(1);
				printf("    grand child working:%i sec\n", i);
				if (i++ > 5)
					break ;
			}
			printf("  3-2. grand child fin\n");
			exit (0);
		}
		printf(" 2-1. child process\n");
		printf(" 2-2. grand child waiting\n");
//		waitpid(pid2, NULL, 0);
		printf(" 2-3. child process exit\n");
		exit (0);
	}
	printf("1-1. main process\n");

//	waitpid(pid1, NULL, 0);
	while (wait(NULL) > 0);

	printf("1-2. main process fin\n");
	*/

	return (0);
}