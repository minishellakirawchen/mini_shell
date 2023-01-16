/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_filedes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:47:11 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:42:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_fd_new_to_old(int old_pipe_fd[2], const int new_pipe_fd[2])
{
	if (!old_pipe_fd || !new_pipe_fd)
		return ;
	old_pipe_fd[READ] = new_pipe_fd[READ];
	old_pipe_fd[WRITE] = new_pipe_fd[WRITE];
}

void init_pipe_fd(int old_pipe_fd[2], int new_pipe_fd[2])
{
	if (!old_pipe_fd || !new_pipe_fd )
		return ;
	old_pipe_fd[READ] = STDIN_FILENO;
	old_pipe_fd[WRITE] = -1;
	new_pipe_fd[READ] = -1;
	new_pipe_fd[WRITE] = -1;
}

int	handle_fd_for_redirection(t_redirect_info *r_info)
{
	int	fd;

	if (!r_info)
		return (FAILURE);
	if (r_info->input_from == E_STDIN && r_info->ouput_to == E_STDOUT)
		return (SUCCESS);
	errno = 0;
	if (r_info->ouput_to == E_REDIRECT_OUT || r_info->ouput_to == E_REDIRECT_APPEND)
	{
		if (dup2(r_info->r_fd[R_FD_OUTFILE], STDOUT_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(r_info->r_fd[R_FD_OUTFILE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	if (r_info->input_from == E_REDIRECT_IN || r_info->input_from == E_HERE_DOC)
	{
		fd = R_FD_INFILE;
		if (r_info->input_from == E_HERE_DOC)
			fd = R_FD_HEREDOC;
		if (dup2(r_info->r_fd[fd], STDIN_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(r_info->r_fd[fd]) < 0)
			return (perror_and_return_int("close", FAILURE));
//		printf("handler heredoc_file:%s\n", r_info->heredoc_file);
		if (r_info->input_from == E_HERE_DOC && unlink(r_info->heredoc_file) < 0)
			return (perror_and_return_int("unlink", FAILURE));
	}
	return (SUCCESS);
}
