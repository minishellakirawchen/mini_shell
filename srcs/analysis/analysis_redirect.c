/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 08:39:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 17:52:24 by takira           ###   ########.fr       */
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

ssize_t	count_split_point_count_by_chr(const char *str, char chr, bool *is_error)
{
	size_t	j;
	ssize_t	cnt;

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

		// syntax check
		if (str[j + 1] == chr) // <<
			j++;
		if (str[j + 1] == chr) // <<<
		{
			printf("syntax error\n");//TODO:error handling
			*is_error = true;
			return (0);
		}

		//後ろで区切る
		// <B, <<B, A<B, A<<B, A<, A<<
		// ^    ^    ^     ^    ^    ^ j
		if (str[j + 1] && str[j + 1] != chr)
			cnt++;
		j++;
	}
	return (cnt);
}

ssize_t	get_split_redirect_char_size(const char **cmds)
{
	ssize_t	cnt;
	size_t	i;
	bool	is_error;

	cnt = 0;
	is_error = false;
	if (!cmds)
		return (0);
	i = 0;
	while (cmds[i] && !is_error)
	{
		cnt++;
		if (count_chr_in_src(cmds[i], '<') != 0)
			cnt += count_split_point_count_by_chr(cmds[i], '<', &is_error);
		if (count_chr_in_src(cmds[i], '>') != 0)
			cnt += count_split_point_count_by_chr(cmds[i], '>', &is_error);
//		printf("cnt:%zu, cmd[i]:%s\n", cnt, cmds[i]);
		i++;
	}
	if (is_error)
		return (-1);
	return (cnt);
}

// A<, A<B, A<<B, <<B, <B
//  ^   ^    ^      ^   ^
size_t	get_split_idx_by_chr(const char *str, char chr)
{
	size_t	j;

	j = 0;
	while (str[j])
	{
		while (str[j] && str[j] != chr)
			j++;
		if (!str[j])
			break ;
		if (j > 0 && str[j - 1] != chr)
			return (j);
		if (str[j + 1] == chr) // <<
			j++;
		if (str[j + 1] && str[j + 1] != chr)
		{
			j++;
			return (j);
		}
		j++;
	}
	return (j);
}


char	**get_split(const char **cmds, size_t	size)
{
	char	**splitted_cmds;
	size_t	i;
	size_t	j;
	size_t	head_idx;
	size_t	word_size;

	splitted_cmds = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!splitted_cmds)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	j = 0;
	while (cmds[i])
	{
		if (count_chr_in_src(cmds[i], '<') == 0 && count_chr_in_src(cmds[i], '>') == 0)
		{
			splitted_cmds[j] = ft_strdup(cmds[i]);
			if (!splitted_cmds[j])
			{
				perror("malloc");
				free_array(splitted_cmds);
				return (NULL);
			}
			i++;
			j++;
			continue ;
		}
		// "<", "<<", ">", ">>" の先頭側のsplit pointで分割する
		head_idx = 0;
		while (cmds[i][head_idx])
		{
			word_size = minsize(get_split_idx_by_chr(&cmds[i][head_idx], '<'), get_split_idx_by_chr(&cmds[i][head_idx], '>'));
			splitted_cmds[j] = ft_substr(cmds[i], head_idx, word_size);
			if (!splitted_cmds[j])
			{
				perror("malloc");
				free_array(splitted_cmds);
				return (NULL);
			}
			printf("splitted_cmds[i]:%s, head:%zu,size:%zu\n", splitted_cmds[i], head_idx, word_size);
			head_idx += word_size;
			j++;
		}
		i++;
	}
	return (splitted_cmds);
}

char **split_redirect_sign_and_word(const char **cmds)
{
	char	**splitted_cmds;
	size_t	size;

	size = get_split_redirect_char_size(cmds);
	splitted_cmds = get_split(cmds, size);

	return (splitted_cmds);
}

//一度全部見て、last <, <<, >, >>を探す、重複の場合は更新（古い分はviaに入れる）
// ->input_from, output_toが決定

// search <, <<, >, >>
//  last < or << :: input_from
//  last > or >> :: output_to
//  others       :: via_files

// How to search
//  cmds[i] = (*node)->cmds[i] = {"<", "file", "cmd0", "cmd1", ..., NULL}[i]
// 	* 単体  "<", "<<", ">", ">>" :: "<", "infile<"になっている可能性もある...
//	* 結合1 "<infile", "outfile>"
//  * 結合2 "infile>", "<outfile", "<<infile>" ※cmds[i]単独で判別できない
// すべて"space"で結合してながら< > 前後にspaceがなかったら挿入 -> space splitする...?
// 1. 結合 & split by ' '
// 2. cmds[i] == <, <<, >, >> を探し、i++と合わせて処理
// 3. cmds[i] != <, <<, >, >> はchar **cmds_except_redirectに保存
// 4. cmds_except_redirectをcmdsに上書きする
int	add_redirect_param(t_tree **node)
{
	char 	**splitted_cmds;

	if (!node || !*node || (*node)->exe_type != E_LEAF_COMMAND || !(*node)->cmds)
		return (FAILURE);
	splitted_cmds = split_redirect_sign_and_word((const char **)(*node)->cmds);
	debug_print_2d_arr(splitted_cmds, "splitted_cmds");

	return (SUCCESS);

	//redirect関係のcharを削除する
	// execve "" は問題なし、command not foundにはならない
	// $> << end ならhere_docのみ実行されるはず
	// ^^^^^^^^^^^^^^^^^^^^^^^^^
	//minishell $>
	//#DEBUG[print_stack : check pipe case]
	// [root]
	//  |
	// [pipe]
	//  |
	// [cmd]--{}
	//
	//splitted_cmds:{}
	//minishell $>
	// ^^^^^^^^^^^^^^^^^^^^^^^^^
}




















