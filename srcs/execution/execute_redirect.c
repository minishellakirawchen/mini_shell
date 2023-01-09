/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 20:00:25 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 21:08:03 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration */
static int	get_fd_and_open_file(char *filename, t_fopen_type fopen_type);
static int	exec_heredoc(int *fd, char **files);
static int	open_infiles(int *fd, char **files);
static int	open_outfiles(int *fd, char **files, t_output_to output_to);

/* functions */
int	openfile_and_heredoc_for_redirect(t_tree **root)
{
	t_tree			*node;
	t_redirect_info	*r_info;

	if (!root | !*root | !(*root)->next)
		return (FAILURE);

	node = (*root)->next;
	while (node)
	{
		if (node->exe_type == E_LEAF_COMMAND && node->redirect_info)
		{
			// TODO:使わないファイルはcreateするだけ
			// input_from, output_toに合わせてfd_openだけを実行する？
			r_info = node->redirect_info;
			if (r_info->here_doc_limiters)
				if (exec_heredoc(&r_info->fd[FD_IDX_HEREDOC], r_info->here_doc_limiters) == FAILURE)
					return (FILE_OPEN_ERROR);
			if (r_info->infiles)
				if (open_infiles(&r_info->fd[FD_IDX_INFILE], r_info->infiles) == FAILURE)
					return (FILE_OPEN_ERROR);
			if (r_info->outfiles)
				if (open_outfiles(&r_info->fd[FD_IDX_OUTFILE], r_info->outfiles, r_info->ouput_to) == FAILURE)
					return (FILE_OPEN_ERROR);
		}
		node = node->next;
	}
	return (SUCCESS);
}

static int	get_fd_and_open_file(char *filename, t_fopen_type fopen_type)
{
	if (fopen_type == e_read)
		return (open(filename, O_RDONLY));
	if (fopen_type == e_overwrite)
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666));
	return (open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666));
}

// bash: XXX: Permission denied
// bash: XXX: No such file or directory
static int	exec_heredoc(int *fd, char **files)
{
	size_t	idx;

	if (!files)
		return (FAILURE);
	idx = 0;
	while (files[idx])
	{
		// TODO: 仮
		*fd = get_fd_and_open_file(files[idx], e_read);
		if (*fd < 0)
			return (perror_and_return_int("open", FAILURE));
		idx++;
		if (files[idx])
			if (close(*fd) < 0)
				return (perror_and_return_int("close", FAILURE));
	}
	return (SUCCESS);
}

static int	open_infiles(int *fd, char **files)
{
	size_t	idx;

	if (!files)
		return (FAILURE);
	idx = 0;
	while (files[idx])
	{
		*fd = get_fd_and_open_file(files[idx], e_read);
		if (*fd < 0)
			return (perror_and_return_int("open", FAILURE));
		idx++;
		if (files[idx])
			if (close(*fd) < 0)
				return (perror_and_return_int("close", FAILURE));
	}
	return (SUCCESS);
}

static int	open_outfiles(int *fd, char **files, t_output_to output_to)
{
	size_t			idx;
	t_fopen_type	fopen_type;

	if (!files)
		return (FAILURE);

	fopen_type = e_overwrite;
	if (output_to == E_REDIRECT_APPEND)
		fopen_type = e_append;

	idx = 0;
	while (files[idx])
	{
		*fd = get_fd_and_open_file(files[idx], fopen_type);
		if (*fd < 0)
			return (perror_and_return_int("open", FAILURE));
		idx++;
		if (files[idx])
			if (close(*fd) < 0)
				return (perror_and_return_int("close", FAILURE));
	}
	return (SUCCESS);
}

