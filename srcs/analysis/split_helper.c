/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 18:28:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 08:42:59 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// (*node)->cmds = {"<", "file", "cmd0", "cmd1", ..., NULL}
// (*node)->cmds = {"<file", "cmd0", "cmd1", ..., NULL}
// if cmds[i][0]="<"
//   len(cmd[i])==1 -> cmd[i+1]  =file
//   len(cmd[i])>1  -> cmd[i][1:]=file

size_t	count_chr_in_src(const char *src, char chr)
{
	size_t	cnt;
	size_t	idx;

	cnt = 0;
	idx = 0;
	if (!src)
		return (0);
	while (src[idx])
	{
		if (src[idx] == chr)
			cnt++;
		idx++;
	}
	return (cnt);
}

//ssize_t	count_split_point_count_by_chr(const char *str, char chr, bool *is_error)
ssize_t	count_split_point_count_by_chr(const char *str, char chr)
{
	size_t	j;
	ssize_t	cnt;

//	*is_error = false;
	cnt = 0;
	j = 0;
	while (str[j])
	{
		while (str[j] && str[j] != chr)
			j++;
		if (!str[j])
			break ;

		//前で区切る
		// <A, <<A, A<B, A<<B, A<, A<<
		// ^   ^     ^    ^     ^   ^ j
		if (j > 0 && str[j - 1] != chr)
			cnt++;

		while (str[j] && str[j] == chr) // <<
			j++;

		//後ろで区切る
		// <B, <<B, A<B, A<<B, A<, A<<
		// ^    ^    ^     ^    ^    ^ j
		if (str[j + 1] && str[j + 1] != chr)
			cnt++;
		j++;
	}
	return (cnt);
}

// A<, A<B, A<<B, <<B, <B
//  ^   ^    ^      ^   ^ return idx
size_t	get_split_idx_by_chr(const char *str, char chr)
{
	size_t	j;

	j = 0;
	while (str[j] && str[j] != chr)
		j++;
	if (j > 0 && str[j - 1] != chr)
		return (j);
	while (str[j] && str[j] == chr) // <<
		j++;
	return (j);
}
