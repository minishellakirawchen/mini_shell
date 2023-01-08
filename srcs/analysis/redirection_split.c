/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 18:26:48 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 18:32:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prototype declaration */
static int	assign_dest_to_splitted_src(char ***dest, const char *src, size_t *j);
static int	assign_dest_to_src(char ***dest, const char *src, size_t *j);
static char	**split_redirect_and_word(const char **cmds_src, size_t size);

/* Function */
char **split_redirect_and_word_controller(const char **cmds)
{
	char	**splitted_cmds;
	size_t	size;

	size = get_split_redirect_char_size(cmds);
	splitted_cmds = split_redirect_and_word(cmds, size);
	if (!splitted_cmds)
		return (NULL);
	return (splitted_cmds);
}

// "<", "<<", ">", ">>" の先頭側のsplit pointで分割する
static int	assign_dest_to_splitted_src(char ***dest, const char *src, size_t *j)
{
	size_t	head_idx;
	size_t	word_size;

	head_idx = 0;
	while (src[head_idx])
	{
		word_size = minsize(get_split_idx_by_chr(&src[head_idx], '<'), get_split_idx_by_chr(&src[head_idx], '>'));
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

static int	assign_dest_to_src(char ***dest, const char *src, size_t *j)
{
	(*dest)[*j] = ft_strdup(src);
	if (!(*dest)[*j])
	{
		free_array(*dest);
		perror("malloc");
		return (FAILURE);
	}
	*j += 1;
	return (SUCCESS);
}

static char	**split_redirect_and_word(const char **cmds_src, size_t size)
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
		if (count_chr_in_src(cmds_src[i], '<') == 0 && count_chr_in_src(cmds_src[i], '>') == 0)
		{
			if (assign_dest_to_src(&splitted_cmds, cmds_src[i], &j) == FAILURE)
				return (NULL);
			i++;
			continue ;
		}
		if (assign_dest_to_splitted_src(&splitted_cmds, cmds_src[i], &j) == FAILURE)
			return (NULL);
		i++;
	}
	return (splitted_cmds);
}
