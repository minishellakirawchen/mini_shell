/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_delete_infile.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 08:50:25 by takira            #+#    #+#             */
/*   Updated: 2023/01/10 08:56:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "test.h"

int main(void)
{
	// test cmd < infile
	// after dup, delete infile
	char *cat[10] = {"cat", "-e", NULL};
	char *file = "infile";
	int	file_fd[2];

	file_fd[READ] = open(file, O_RDONLY);
	dup2(file_fd[READ], STDIN_FILENO);
	close(file_fd[READ]);
	unlink(file);
	execvp(cat[0], cat);
	return (0);
}
