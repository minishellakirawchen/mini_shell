/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:36:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 16:54:00 by takira           ###   ########.fr       */
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
	char *ls[10] = {"ls", "-l", NULL};
	char *file = "outfile";
	int	file_fd[2];

	file_fd[WRITE] = open(file, O_WRONLY);
	dup2(file_fd[WRITE], STDOUT_FILENO);
	execvp(ls[0], ls);

	return (0);
}
