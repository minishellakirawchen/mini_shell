/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:36:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 18:44:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void)
{
	// test cmd < infile
	/*
	char *cat[10] = {"cat", "-e", NULL};
	char *file = "infile";
	int	file_fd[2];

	file_fd[READ] = open(file, O_RDONLY);
	dup2(file_fd[READ], STDIN_FILENO);
	execvp(cat[0], cat);
	*/

	// test cmd > outfile
	/*
	char *ls[10] = {"ls", "-l", NULL};
	char *file = "outfile";
	int	file_fd[2];

	file_fd[WRITE] = open(file, O_WRONLY);
	dup2(file_fd[WRITE], STDOUT_FILENO);
	execvp(ls[0], ls);
	*/

	// test cmd > via1 > via2 > out
	char *ls[10] = {"ls", "-l", NULL};
	char *via1 = "via1";
	char *via2 = "via2";
	char *out = "out";
	int fd_via1, fd_via2, fd_out;

	fd_via1 = open(via1, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	fd_via2 = open(via2, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	fd_out = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0666);

	dup2(fd_via1, STDOUT_FILENO);
	close(fd_via1);
	dup2(fd_via2, STDOUT_FILENO);
	close(fd_via2);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);

	execvp(ls[0], ls);

	return (0);
}
