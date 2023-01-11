/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrangement_command_line.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 19:15:40 by takira            #+#    #+#             */
/*   Updated: 2023/01/11 22:56:56 by takira           ###   ########.fr       */
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

bool	is_option_str(const char *str)
{
	size_t	idx;

	if (!str)
		return (false);
	if (str[0] != CHR_CMD_OPTION_FLG || ft_strlen_ns(str) <= 1)
		return (false);
	idx = 1;
	while (str[idx])
	{
		if (!ft_isalpha(str[idx]))
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

// option_str = "-[a-zA-Z]"
char	*create_option_str(const int *cnt_arr)
{
	char	*option_str;
	int		cnt_idx;
	size_t	size;
	size_t	str_idx;
	char	chr;

	cnt_idx = 0;
	size = 0;
	while (cnt_idx < ALPHABET_CNT * 2)
	{
		if (cnt_arr[cnt_idx] > 0)
			size++;
		cnt_idx++;
	}
	if (size == 0)
		return (NULL);
	option_str = (char *)malloc(sizeof(char) * (size + 2));
	if (!option_str)
		return (perror_and_return_nullptr("malloc"));
	option_str[0] = CHR_CMD_OPTION_FLG;
	option_str[size + 1] = '\0';
	cnt_idx = 0;
	str_idx = 1;
	while (size)
	{
		if (cnt_arr[cnt_idx] > 0)
		{
			chr = (char)(cnt_idx + 'a');
			if (cnt_idx >= ALPHABET_CNT)
				chr = ft_toupper(chr);
			printf("chr:%c, cnt:%d\n", chr, cnt_arr[cnt_idx]);
			option_str[str_idx] = chr;
			size--;
			str_idx++;
		}
		cnt_idx++;
	}
	printf("option str:%s\n", option_str);
	return (option_str);
}

ssize_t	get_option_count(int **cnt_arr, const char **src)
{
	ssize_t	cnt;
	size_t	idx;

	if (!cnt_arr || !*cnt_arr || !src || !*src)
		return (-1);
	idx = 1;
	cnt = 0;
	while (src[idx] && is_option_str(src[idx]))
	{
		cnt++;
		count_cmd_options(cnt_arr, src[idx]);
		printf("is_option %s\n", src[idx]);
		idx++;
	}
	return (cnt);
}


// echo -a -b -c hoge option:3
//
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

	arranged_cmds[dst_idx] = ft_strdup_ns(src[src_idx]);
	if (!arranged_cmds[dst_idx])
	{
		free_2d_array_ret_nullptr((void ***)&arranged_cmds);
		return (perror_and_return_nullptr("malloc"));
	}

	dst_idx++;
	src_idx++;

	arranged_cmds[dst_idx] = create_option_str(cnt_arr);
	printf("option:%s, option cnt:%zu\n", arranged_cmds[dst_idx], option_str_cnt);
	if (!arranged_cmds[dst_idx])
	{
		free_2d_array_ret_nullptr((void ***)&arranged_cmds);
		return (perror_and_return_nullptr("malloc"));
	}
	dst_idx++;

	src_idx += option_str_cnt;
	while (src[src_idx])
	{
		arranged_cmds[dst_idx] = ft_strdup_ns(src[src_idx]);
		if (!arranged_cmds[dst_idx])
		{
			free_2d_array_ret_nullptr((void ***)&arranged_cmds);
			return (perror_and_return_nullptr("malloc"));
		}
		src_idx++;
		dst_idx++;
	}
	return (arranged_cmds);
}

char **arranged_cmds(char **src)
{
	char			**arranged_cmds;
	int				*option_cnt_arr;
	ssize_t			option_str_cnt;

	if (!src || !*src)
		return (NULL);
	printf("1\n");
	option_cnt_arr = (int *)ft_calloc(sizeof(int), ALPHABET_CNT * 2);
	if (!option_cnt_arr)
		return ((char **)perror_and_return_nullptr("malloc"));
	printf("2\n");

	option_str_cnt = get_option_count(&option_cnt_arr, (const char **)src);
	if (option_str_cnt == 0)
		return (src);
	printf("3\n");

	arranged_cmds = create_arranged_cmds(src, option_str_cnt, option_cnt_arr);
	if (!arranged_cmds)
	{
		free_1d_array_ret_nullptr((void **)&option_cnt_arr);
		return (NULL);
	}
	printf("4\n");

	debug_print_2d_arr(src, "src");
	debug_print_2d_arr(arranged_cmds, "arranged");
	free_2d_array_ret_nullptr((void ***)&src);
	free_1d_array_ret_nullptr((void **)&option_cnt_arr);
	return (arranged_cmds);
}

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
			node->cmds = arranged_cmds(node->cmds);
			if (!node->cmds)
				return (FAILURE);
		}
		node = node->next;
	}
	debug_print_stack(info->tree_root, "arrange command line");
	return (SUCCESS);
}