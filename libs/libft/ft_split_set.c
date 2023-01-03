/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:10:04 by takira            #+#    #+#             */
/*   Updated: 2022/12/09 12:10:05 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
	return (NULL);
}

static void	get_strs_size(const char *str, char delim, char set, size_t *cnt)
{
	size_t	set_cnt;
	size_t	i;

	set_cnt = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == delim)
			i++;
		if (str[i] && str[i] != set)
		{
			if (set_cnt % 2 == 0)
				*cnt += 1;
			while (str[i] && str[i] != delim && str[i] != set)
				i++;
		}
		if (str[i] && str[i] == set)
		{
			set_cnt++;
			if (set_cnt % 2 == 1)
				*cnt += 1;
			i++;
		}
	}
}

static size_t	get_split_idx(const char *str, char dlm, char set, size_t *head)
{
	size_t	tail;
	bool	is_in_set;

	while (str[*head] && str[*head] == dlm)
		*head += 1;
	is_in_set = false;
	if (str[*head] == set)
		is_in_set = true;
	tail = *head;
	if (is_in_set)
		tail++;
	if (is_in_set)
		while (str[tail] && str[tail] != set)
			tail++;
	else
		while (str[tail] && str[tail] != dlm && str[tail] != set)
			tail++;
	return (tail);
}

static char	**get_split_strs(char **strs, const char *str, char delim, char set)
{
	size_t	i;
	size_t	head_idx;
	size_t	tail_idx;
	size_t	head_shift;

	i = 0;
	head_idx = 0;
	while (str[head_idx])
	{
		head_shift = 0;
		tail_idx = get_split_idx(str, delim, set, &head_idx);
		if (head_idx >= tail_idx || (str[head_idx] == delim && !str[tail_idx]))
			break ;
		if (str[head_idx] == set)
			head_shift = 1;
		head_idx += head_shift;
		strs[i] = ft_substr(str, head_idx, tail_idx - head_idx);
		if (!strs[i])
			return (free_array(strs));
		i++;
		head_idx = tail_idx;
		if (str[tail_idx])
			head_idx += head_shift;
	}
	return (strs);
}

char	**ft_split_set(const char *str, char delim, char set)
{
	char	**strs;
	size_t	strs_size;

	if (!str || delim == set)
		return (NULL);
	strs_size = 0;
	if (delim)
		get_strs_size(str, delim, set, &strs_size);
	strs = (char **)ft_calloc(sizeof(char *), strs_size + 1);
	if (!strs)
		return (NULL);
	get_split_strs(strs, str, delim, set);
	if (!strs)
		return (NULL);
	strs[strs_size] = NULL;
	return (strs);
}
