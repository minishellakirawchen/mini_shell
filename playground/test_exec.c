/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:36:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/18 02:07:56 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void)
{
	// test cmd < infile

	// char *cat[10] = {"cat", "-e", NULL};
	// char *file = "infile";
	// int	file_fd[2];
	// char *buf = NULL;
	// ssize_t read_num;

	// file_fd[READ] = open(file, O_RDONLY);
	// printf("file_fd[READ]: %d\n", file_fd[READ]);
	// dup2(file_fd[READ], STDIN_FILENO);
	// read_num = read(file_fd[READ], buf, 7); //is closed that can not be used
	// printf("read_num: %zd\n", read_num);
	// printf("file_fd[READ]: %d\n", file_fd[READ]);
	// execvp(cat[0], cat);


	// test cmd > outfile

	// char *ls[10] = {"ls", "-l", NULL};
	// char *file = "outfile";
	// int	file_fd[2];

	// file_fd[WRITE] = open(file, O_WRONLY | O_TRUNC | O_CREAT);
	// dup2(file_fd[WRITE], STDOUT_FILENO);
	// execvp(ls[0], ls);


	// test cmd > via1 > via2 > out

	// char *ls[10] = {"ls", "-l", NULL};
	// char *via1 = "via1";
	// char *via2 = "via2";
	// char *out = "out";
	// int fd_via1, fd_via2, fd_out;

	// fd_via1 = open(via1, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	// fd_via2 = open(via2, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	// fd_out = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0666);

	// dup2(fd_via1, STDOUT_FILENO);
	// //close(fd_via1); //closeする意味（dup2したら自動に閉じるでは！？）
	// dup2(fd_via2, STDOUT_FILENO);
	// //close(fd_via2);
	// dup2(fd_out, STDOUT_FILENO);
	// //close(fd_out);

	// execvp(ls[0], ls);


	// Try this
	//$ < in1 cat > out1 > out2 < in2
	//$ cat out2
	//$test2
//start
	char *cat[10] = {"cat", NULL};
	char *in1 = "in1";
	char *in2 = "vin2";
	char *out1 = "out1";
	char *out2 = "out2";
	int fd_in1, fd_in2, fd_out1, fd_out2;

	int		pipe_fd[2];
	pid_t	pid;

	fd_in1 = open(in1, O_RDONLY);
	fd_in2 = open(in2, O_RDONLY);
	fd_out1 = open(out1, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	fd_out2 = open(out2, O_CREAT | O_WRONLY | O_TRUNC, 0666);


	printf("fd_in1:%d \n", fd_in1);
	printf("fd_in2:%d \n", fd_in2);
	printf("fd_out1:%d \n", fd_out1);
	printf("fd_out2:%d \n", fd_out2);
	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_in1, STDIN_FILENO);
		//close(fd_in1);

		close(pipe_fd[READ]);
		close(STDOUT_FILENO);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		//close(pipe_fd[WRITE]);

		execvp(cat[0], cat);
		perror("execvp");
		exit (1);
	}
	close(pipe_fd[WRITE]);
	close(STDIN_FILENO);
	dup2(pipe_fd[READ], STDIN_FILENO);
	//close(pipe_fd[READ]);
	wait(NULL);


	dup2(fd_out1, STDOUT_FILENO);
	//close(fd_out1);

	dup2(fd_in2, STDIN_FILENO);
	//close(fd_in2);

	dup2(fd_out2, STDOUT_FILENO);
	//close(fd_out2);

	return (0);
}
