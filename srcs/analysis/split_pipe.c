/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 08:36:40 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 08:47:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prototype declaration */
static int		assign_dest_to_splitted_src(char ***dest, const char *src, size_t *j);
static char		**split_pipe_and_word(const char **cmds_src, size_t size);
static ssize_t	get_split_pipe_char_size(const char **cmds);

/* Function */
char **split_pipe_and_word_controller(const char **cmds)
{
	char	**splitted_cmds;
	size_t	size;

	size = get_split_pipe_char_size(cmds);
	splitted_cmds = split_pipe_and_word(cmds, size);
	if (!splitted_cmds)
		return (NULL);
	return (splitted_cmds);
}

// "|" の先頭側のsplit pointで分割する
static int	assign_dest_to_splitted_src(char ***dest, const char *src, size_t *j)
{
	size_t	head_idx;
	size_t	word_size;

	head_idx = 0;
	while (src[head_idx])
	{
		word_size = get_split_idx_by_chr(&src[head_idx], '|');
		(*dest)[*j] = ft_substr(src, head_idx, word_size);
		if (!(*dest)[*j])
		{
			free_array(*dest);
			perror("malloc");
			return (FAILURE);
		}
		head_idx += word_size;
		*j += 1;
	}
	return (SUCCESS);
}

static char	**split_pipe_and_word(const char **cmds_src, size_t size)
{
	char	**splitted_cmds;
	size_t	i;
	size_t	j;

	splitted_cmds = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!splitted_cmds)
		return ((char **) perror_and_ret_nullptr("malloc"));
	i = 0;
	j = 0;
	while (cmds_src[i])
	{
		if (assign_dest_to_splitted_src(&splitted_cmds, cmds_src[i], &j) == FAILURE)
			return (NULL);
		i++;
	}
	return (splitted_cmds);
}

static ssize_t	get_split_pipe_char_size(const char **cmds)
{
	ssize_t	cnt;
	size_t	i;

	cnt = 0;
	if (!cmds)
		return (0);
	i = 0;
	while (cmds[i])
	{
		cnt++;
		if (count_chr_in_src(cmds[i], '|') != 0)
			cnt += count_split_point_count_by_chr(cmds[i], '|');
//		printf("cnt:%zu, cmd[i]:%s\n", cnt, cmds[i]);
		i++;
	}
	return (cnt);
}

