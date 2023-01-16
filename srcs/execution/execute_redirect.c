/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 20:00:25 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:41:31 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration */
int			do_heredoc_for_redirect(t_tree **root);
int			open_file_for_redirect(t_tree **root);
char		*get_heredoc_tmp_filename(int cnt);
static int	open_file_controller(t_redirect_info **r_info);
static int	get_fd_and_open_file(const char *filename, t_fopen_type fopen_type);
static int	create_heredoc_file(t_redirect_info **r_info, int cnt);

/* functions */

int	execute_redirect(t_tree **root)
{
	// heredocが先
	if (do_heredoc_for_redirect(root) == FAILURE)
		return (FAILURE);

	// openfileが後
	if (open_file_for_redirect(root) == FAILURE)
		return (FAILURE);

	return (SUCCESS);
}

int do_heredoc_for_redirect(t_tree **root)
{
	t_tree			*node;
	t_redirect_info	*r_info;
	int				tmp_cnt;

	if (!root | !*root | !(*root)->next)
		return (FAILURE);
	tmp_cnt = 0;
	node = (*root)->next;
	while (node)
	{
		if (node->exe_type == E_LEAF_COMMAND)
		{
			r_info = node->redirect_info;
			if (r_info && r_info->heredoc_delims)
			{
				if (create_heredoc_file(&r_info, tmp_cnt) == FAILURE)
					return (FILE_OPEN_ERROR);
				tmp_cnt++;
			}
		}
		node = node->next;
	}
	return (SUCCESS);
}

static int	create_heredoc_file(t_redirect_info **r_info, int cnt)
{
	size_t			idx;

	errno = 0;
	if (!r_info || !*r_info || !(*r_info)->heredoc_delims)
		return (FAILURE);
	(*r_info)->heredoc_file = get_heredoc_tmp_filename(cnt);
	if (!(*r_info)->heredoc_file)
		return (FAILURE);
	idx = 0;
	while ((*r_info)->heredoc_delims[idx])
	{
		(*r_info)->r_fd[R_FD_HEREDOC] = get_fd_and_open_file((*r_info)->heredoc_file, e_overwrite);
		if ((*r_info)->r_fd[R_FD_HEREDOC] < 0)
			return (perror_and_return_int("open", FAILURE));
		if (execute_heredoc((*r_info)->r_fd[R_FD_HEREDOC], (*r_info)->heredoc_delims[idx]) == FAILURE)
			return (EXIT_FAILURE);
		if (close((*r_info)->r_fd[R_FD_HEREDOC]) < 0)
			return (perror_and_return_int("close", FAILURE));
		idx++;
	}
	if ((*r_info)->input_from == E_HERE_DOC)
	{
		(*r_info)->r_fd[R_FD_HEREDOC] = get_fd_and_open_file((*r_info)->heredoc_file, e_read);
		if ((*r_info)->r_fd[R_FD_HEREDOC] < 0)
			return (perror_and_return_int("open", FAILURE));
	//	ft_printf("file:%s, fd:%d\n", *heredoc_file, *fd);
	}
	return (SUCCESS);
}

int open_file_for_redirect(t_tree **root)
{
	t_tree			*node;
	t_redirect_info	*r_info;

	if (!root | !*root | !(*root)->next)
		return (FAILURE);
	node = (*root)->next;
	while (node)
	{
		if (node->exe_type == E_LEAF_COMMAND)
		{
			r_info = node->redirect_info;
			if (!r_info)
				return (FAILURE);
			if (open_file_controller(&r_info) == FAILURE)
				return (FILE_OPEN_ERROR);
		}
		node = node->next;
	}
	return (SUCCESS);

}

static int	open_files(int *fd, const char **files, t_fopen_type type)
{
	size_t	idx;

	errno = 0;
	if (!files)
		return (FAILURE);
	idx = 0;
	while (files[idx])
	{
		*fd = get_fd_and_open_file(files[idx], type);
		if (*fd < 0)
			return (perror_and_return_int("open", FAILURE));
		idx++;
		if (files[idx])
			if (close(*fd) < 0)
				return (perror_and_return_int("close", FAILURE));
	}
	return (SUCCESS);
}

static int	open_file_controller(t_redirect_info **r_info)
{
	t_fopen_type	fopen_type;

	errno = 0;
	if (!r_info || !*r_info)
		return (FAILURE);
	if ((*r_info)->input_from == E_REDIRECT_IN)
	{
		fopen_type = e_read;
		if (open_files(&(*r_info)->r_fd[R_FD_INFILE], (const char **)(*r_info)->infiles, fopen_type) == FAILURE)
			return (FAILURE);
		if ((*r_info)->input_from == E_HERE_DOC)
			if (close((*r_info)->r_fd[R_FD_INFILE]) < 0)
				return (perror_and_return_int("close", FAILURE));
	}
	if ((*r_info)->ouput_to == E_REDIRECT_OUT || (*r_info)->ouput_to == E_REDIRECT_APPEND)
	{
		fopen_type = e_overwrite;
		if ((*r_info)->ouput_to == E_REDIRECT_APPEND)
			fopen_type = e_append;
		if (open_files(&(*r_info)->r_fd[R_FD_OUTFILE], (const char **)(*r_info)->outfiles, fopen_type) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

static int	get_fd_and_open_file(const char *filename, t_fopen_type fopen_type)
{
	if (fopen_type == e_read)
		return (open(filename, O_RDONLY));
	if (fopen_type == e_overwrite)
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666));
	return (open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666));
}

// tmpfile: here_doc_<cnt>.tmp
char	*get_heredoc_tmp_filename(int cnt)
{
	char	*strcnt;
	char 	*filename;
	size_t	tmp_len;
	size_t	strcnt_len;

	errno = 0;
	strcnt = ft_itoa(cnt);
	if (!strcnt)
		return (perror_and_return_nullptr("malloc"));
	tmp_len = ft_strlen_ns(HEREDOC_TMP_FILE);
	strcnt_len = ft_strlen_ns(strcnt);
	filename = (char *)ft_calloc(sizeof(char) ,(tmp_len + strcnt_len + 1));
	if (!filename)
		return (perror_and_return_nullptr("malloc"));
	ft_strlcat(filename, HEREDOC_TMP_FILE, tmp_len + strcnt_len + 1);
	ft_strlcat(filename, strcnt, tmp_len + strcnt_len + 1);
	free(strcnt);
	return (filename);
}

// bash: XXX: Permission denied
// bash: XXX: No such file or directory
//static int	create_heredoc_file(int *fd, const char **delimiters, char **heredoc_file, int cnt)
//{
//	size_t	idx;
//
//	if (!delimiters)
//		return (FAILURE);
//	idx = 0;
//	*heredoc_file = get_heredoc_tmp_filename(cnt);
//	if (!*heredoc_file)
//		return (FAILURE);
//	while (delimiters[idx])
//	{
//		// pipe内部で実行されるためファイル名の競合は発生しないはず...?
//		*fd = get_fd_and_open_file(*heredoc_file, e_overwrite);
//		if (*fd < 0)
//			return (perror_and_return_int("open", FAILURE));
//		// TODO: exec_here_doc (2);; 1 or 2 ?
//		// TODO: delete tmpfile
//
//		if (execute_heredoc(*fd, delimiters[idx]) == FAILURE)
//			return (EXIT_FAILURE);
//		if (close(*fd) < 0)
//			return (perror_and_return_int("close", FAILURE));
//		idx++;
//	}
////	printf("file:%s, fd:%d\n", *heredoc_file, *fd);
//	*fd = get_fd_and_open_file(*heredoc_file, e_read);
//	if (*fd < 0)
//		return (perror_and_return_int("open", FAILURE));
//	return (SUCCESS);
//}

/*
static int	open_infiles(int *fd, const char **files)
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

static int	open_outfiles(int *fd, const char **files, t_output_to output_to)
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

*/