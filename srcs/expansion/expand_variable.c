/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:06:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/10 22:43:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// name=value; //TODO:valueの定義
// name=[_a-zA-z][_a-zA-Z0-9]
int	is_name(const char *str)
{
	size_t	idx;

	if (!str || !str[0])
		return (0);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (0);
	idx = 1;
	while (str[idx])
	{
		if (str[idx] != '_' && !ft_isalnum(str[idx]))
			return (0);
		idx++;
	}
	return (1);
}

// command option -[a-zA-Z], delete duplicated and continues one

// $test=TEST
// before:{"echo", "hello", ""foo"", "bar", "'hoge'", "$test", NULL}
// after :{"echo", "hello", ""foo"", "bar", "'hoge'", "TEST", NULL}

// $cmd=echo
// before:{"$cmd", "hello", NULL}
// after :{"echo", "hello", NULL}

// $test=ech
// before:{"$test", "o", "hello", NULL}
// after :{"ech", "o", "hello", NULL}

// $test=ech
// before:{""$test"", "o", "hello", NULL}
// after :{"echo", "hello", NULL} <- "$test" + o = "ech" + o = echo; expand ""



// $test=echo
// before:{""$test"", "o", "hello", NULL}
// after :{"echo", "hello", NULL}


// How to implement
//  search in word; char *word
//    word = $var
//    word = "$var"
//    word = abc$var  //"$var abc" already separated to "$var" + "abc"
//    word = $
//    word = $$
//    word = $?
//
//  $ only:何もしない
//  $$    :replace with pid
//  $key  :replace with get_env_val(key)
//  $?    :exit status


// TODO: heredoc
// expand var for here_doc
//  exec when write to here_doc_tmp file.

// TODO:echoやexport内部で""の消去を実施, bonusではwordごとにflagをつけた方が良いかも

// ''内以外の$+hogeを探し、見つけたらtrue
// 'だったら次の'まで飛ばす
bool is_expand_needed(const char *str)
{
	size_t	idx;

	if (!str)
		return (false);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_SINGLE_QUOTE)
		{
			idx++;
			while (str[idx] && str[idx] == CHR_SINGLE_QUOTE)
				idx++;
			if (str[idx])
				idx++;
			continue ;
		}
		if (str[idx] == CHR_DOLLAR && str[idx + 1])
		{
			if (str[idx + 1] == CHR_DOLLAR || str[idx + 1] == CHR_QUESTION)
				return (true);
			if (is_name(&str[idx + 1]))
				return (true);
		}
	}
	return (false);
}

// ''内以外の$+hogeを探し、見つけたらtrue
// 'だったら次の'まで飛ばす
size_t	get__next_expand_idx(const char *str)
{
	size_t	idx;

	if (!str)
		return (false);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_SINGLE_QUOTE)
		{
			idx++;
			while (str[idx] && str[idx] == CHR_SINGLE_QUOTE)
				idx++;
			if (str[idx])
				idx++;
			continue ;
		}
		if (str[idx] == CHR_DOLLAR && str[idx + 1])
		{
			if (str[idx + 1] == CHR_DOLLAR || str[idx + 1] == CHR_QUESTION)
				return (idx);
			if (is_name(&str[idx + 1]))
				return (idx);
		}
	}
	return (idx);
}

size_t	get_expand_size(t_info *info, const char *str)
{
	size_t	size;
	size_t	idx;

	if (!str)
		return (0);
	size = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_DOLLAR)
		{
			// TODO
		}
		size++;
		idx++;
	}
	return (size);
}

int	expand_variable_in_str(t_info *info, char **src)
{
	char	*expanded_str;
	size_t	size;

	if (!src || !*src)
		return (FAILURE);
	size = get_expand_size(info, *src);
	expanded_str = (char *)malloc(sizeof(char) * (size + 1));
	if (!expanded_str)
		return (perror_and_return_int("malloc", FAILURE));

	//TODO

	free_1d_array_ret_nullptr((void **)&src);
	*src = expanded_str;
	return (SUCCESS);
}

// ''内部のみ展開しない
// 展開するバリエーション
//  $name = var
//  $?    = exit status
//  $$    = pid;use getpid()
//  他はエラーにする
int	expansion(t_info *info)
{
	t_tree	*node;
	size_t	idx;

	if (!info || !info->tree_root)
		return (FAILURE);
	node = info->tree_root;
	while (node)
	{
		if (node->exe_type == E_LEAF_COMMAND && node->cmds)
		{
			idx = 0;
			while (node->cmds[idx])
			{
				if (is_expand_needed(node->cmds[idx]))
					if (expand_variable_in_str(info, &node->cmds[idx]) == FAILURE)
						return (FAILURE);
				idx++;
			}
		}
		node = node->next;
	}
	return (SUCCESS);
}










