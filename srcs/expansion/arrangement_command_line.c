/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrangement_command_line.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 19:15:40 by takira            #+#    #+#             */
/*   Updated: 2023/01/11 23:58:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1.reshape option
//   cmds: {"echo", "-n", "-n", "-n", "hello", NULL} ->  {"echo", "-n", "hello", NULL}
//   cmds: {"echo", "-nnnnnnnn", "hello", NULL}      ->  {"echo", "-n", "hello", NULL}

// op : "-[a-zA-Z]", 26*2種類だけ
// cmds[i]; i=1 から順番に, cmds[i][0] = -[a-zA-Z]が続くところまでチェックして、option配列で各アルファベットの個数を数える
// optionを結合した文字列をopsとして、以下のように整形する
// arranged_cmds = {cmds[0], ops, cmds[j], cmds[j+1], ..., NULL}
// --optionは扱わない

// ft_builtinはoptionのvalidationする
// 他は成り行きとする

// 2. remove "", ''
//   inside command, such as ft_echo. ...?


// cmd -xxxxx -x -y -x hoge, cmd -xxx-yyy
//  op ^^^^^^ ^^ ^^ ^^           ^^^^^^^^ NOT option

bool	is_option_str(const char *str, const char *options)
{
	size_t	idx;

	if (!str)
		return (false);
	if (str[0] != CHR_CMD_OPTION_FLG || ft_strlen_ns(str) <= 1)
		return (false);
	idx = 1;
	while (str[idx])
	{
		if (ft_strchr(options, str[idx]) == NULL)
			return (false);
		idx++;
	}
	return (true);
}

void	count_cmd_options(int **option_cnt_arr, const char *option_str)
{
	size_t	idx;
	int		ord;
	char	chr;

	if (!option_cnt_arr || !*option_cnt_arr)
		return ;
	if (!option_str || option_str[0] != CHR_CMD_OPTION_FLG)
		return ;
	idx = 1;
	while (option_str[idx])
	{
		chr = option_str[idx];
		if (ft_isalpha(chr))
		{
			ord = (int)ft_tolower(chr) - 'a';
			if (ft_isupper(chr))
				ord += ALPHABET_CNT;
			(*option_cnt_arr)[ord]++;
		}
		idx++;
	}
}

size_t	count_option_aplhabets(const int *cnt_arr)
{
	int		cnt_idx;
	size_t	size;

	cnt_idx = 0;
	size = 0;
	while (cnt_idx < ALPHABET_CNT * 2)
	{
		if (cnt_arr[cnt_idx] > 0)
			size++;
		cnt_idx++;
	}
	return (size);
}

void	add_option_alphabet_to_str(char **str, const int *cnt_arr, size_t strsize)
{
	int		cnt_idx;
	size_t	str_idx;
	char	chr;

	if (!str || !*str)
		return ;
	(*str)[0] = CHR_CMD_OPTION_FLG;
	(*str)[strsize + 1] = '\0';
	cnt_idx = 0;
	str_idx = 1;
	while (strsize)
	{
		if (cnt_arr[cnt_idx] > 0)
		{
			chr = (char)(cnt_idx + 'a');
			if (cnt_idx >= ALPHABET_CNT)
				chr = ft_toupper(chr);
			(*str)[str_idx] = chr;
			strsize--;
			str_idx++;
		}
		cnt_idx++;
	}
}

// option_str = "-[a-zA-Z]"
// option echo -n なので nだけ対応する
char	*create_option_str(const int *cnt_arr)
{
	char	*option_str;
	size_t	size;

	size = count_option_aplhabets(cnt_arr);
	if (size == 0)
		return (NULL);
	option_str = (char *)malloc(sizeof(char) * (size + 2));
	if (!option_str)
		return (perror_and_return_nullptr("malloc"));
	add_option_alphabet_to_str(&option_str, cnt_arr, size);
	return (option_str);
}

ssize_t	get_option_count(int **cnt_arr, const char **src, const char *options)
{
	ssize_t	cnt;
	size_t	idx;

	if (!cnt_arr || !*cnt_arr || !src || !*src)
		return (-1);
	idx = 1;
	cnt = 0;
	while (src[idx] && is_option_str(src[idx], options))
	{
		cnt++;
		count_cmd_options(cnt_arr, src[idx]);
		printf("is_option %s\n", src[idx]);
		idx++;
	}
	return (cnt);
}


// echo -a -b -c hoge   option:3
// echo -nnnnn hoge     option:1

char	**create_arranged_cmds(char **src, ssize_t option_str_cnt, int *cnt_arr)
{
	size_t			src_idx;
	size_t			dst_idx;
	char			**arranged_cmds;
	const size_t	srcsize = get_2d_array_size((const char **)src);

	arranged_cmds = (char **)ft_calloc(sizeof(char *),  srcsize - option_str_cnt + 2);
	if (!arranged_cmds)
		return (perror_and_return_nullptr("malloc"));
	src_idx = 0;
	dst_idx = 0;
	while (src[src_idx])
	{
		if (dst_idx == 1)
			arranged_cmds[dst_idx] = create_option_str(cnt_arr);
		else
			arranged_cmds[dst_idx] = ft_strdup_ns(src[src_idx]);
		if (!arranged_cmds[dst_idx])
		{
			free_2d_array_ret_nullptr((void ***)&arranged_cmds);
			return (perror_and_return_nullptr("malloc"));
		}
		if (dst_idx == 1)
			src_idx += (option_str_cnt - 1);
		src_idx++;
		dst_idx++;
	}
	return (arranged_cmds);
}

char **create_arranged_cmds_controller(char **src, const char *options)
{
	char			**arranged_cmds;
	int				*option_cnt_arr;
	ssize_t			option_str_cnt;

	if (!src || !*src)
		return (NULL);
	option_cnt_arr = (int *)ft_calloc(sizeof(int), ALPHABET_CNT * 2);
	if (!option_cnt_arr)
		return ((char **)perror_and_return_nullptr("malloc"));
	option_str_cnt = get_option_count(&option_cnt_arr, (const char **)src, options);
	if (option_str_cnt == 0)
		return (src);
	arranged_cmds = create_arranged_cmds(src, option_str_cnt, option_cnt_arr);
	if (!arranged_cmds)
	{
		free_1d_array_ret_nullptr((void **)&option_cnt_arr);
		return (NULL);
	}
	free_2d_array_ret_nullptr((void ***)&src);
	free_1d_array_ret_nullptr((void **)&option_cnt_arr);
	return (arranged_cmds);
}

// if all command option reshape, use arrange_command_line before execution part.
/*
int	arrange_command_line(t_info *info)
{
	t_tree	*node;

	if (!info | !info->tree_root)
		return (FAILURE);
	node = info->tree_root;
	while (node)
	{
		if (node->exe_type == E_LEAF_COMMAND)
		{
			node->cmds = create_arranged_cmds_controller(node->cmds, STR_CMD_ECHO_OPTIONS);
			if (!node->cmds)
				return (FAILURE);
		}
		node = node->next;
	}
//	debug_print_stack(info->tree_root, "arrange command line");
	return (SUCCESS);
}
*/