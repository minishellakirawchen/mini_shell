/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:31:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 11:21:05 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// input: hogehoge\n
// delim: hogehoge
bool	is_delimiter(const char *input_line, const char *delimiter)
{
	size_t	input_len;
	size_t	delim_len;

	if (!input_line || !delimiter)
		return (false);
	input_len = ft_strlen_ns(input_line);
	delim_len = ft_strlen_ns(delimiter);
	if ((input_len == delim_len + 1) \
	&& ft_strncmp_ns(input_line, delimiter, delim_len) == 0)
		return (true);
	return (false);
}

bool	is_eof(char *line)
{
	return (!line);
}

int	execute_heredoc(int fd, const char *delimiter)
{
	char	*line;

	if (fd < 0 || !delimiter)
		return (FAILURE);
	while (true)
	{
		ft_dprintf(STDERR_FILENO, "> ");
		line = get_next_line(STDIN_FILENO, true);
		if (is_eof(line))
			return (SUCCESS);// EOF
		if (is_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_dprintf(fd, line);//TODO:expansion
		free_1d_array_ret_nullptr((void **)&line);
	}
	return (SUCCESS);
}
