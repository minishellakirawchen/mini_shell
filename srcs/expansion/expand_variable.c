/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:06:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 17:41:51 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// name=value; //TODO:valueの定義
// name=[_a-zA-z][_a-zA-Z0-9]
// str=$hoo$var$hoge
//      ^^^ ここまでを判定して return (1);
int	is_name(const char *str)
{
	size_t	idx;

	if (!str || !str[0])
		return (0);
	if (str[0] != '_' && !ft_isalpha(str[0]))
		return (0);
	idx = 1;
//	while (str[idx])
	while (str[idx] && str[idx] != CHR_DOLLAR)
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
			if (is_name(&str[idx + 1])) //次の$hogeまで is_name
				return (true);
		}
		idx++;
	}
	return (false);
}

// ''内以外の$+hogeを探し、見つけたらtrue
// 'だったら次の'まで飛ばす

// $hoge,  abc$?,  abc$
// ^          ^        ^(null)
size_t	get_first_expand_sign_idx(const char *str)
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
			if (is_name(&str[idx + 1])) //次の$hogeまで is_name
				return (idx);
		}
		idx++;
	}
	return (idx);
}

// $hogehoge$foo expandable -> true
// ^^^^^^^^^
// ^start

// $? expandable -> true
// ^start

// $  -> false
// ^start

// hoge$huge false
// ^^^^
// ^start
bool	is_expandable_from_start_idx(const char *str, size_t start_idx)
{
	if (!str || ft_strlen_ns(str) <= start_idx + 1)
		return (false);
	if (str[start_idx] != CHR_DOLLAR || !str[start_idx + 1])
		return (false);
	if (str[start_idx + 1] == CHR_DOLLAR)
		return (true);
	if (str[start_idx + 1] == CHR_QUESTION)
		return (true);
	if (is_name(&str[start_idx + 1])) //次の$hogeまで is_name
		return (true);
	return (false);
}

char	*get_next_envkey_in_str(const char *str)
{
	char	*key;;
	size_t	head_idx;
	size_t	tail_idx;
	size_t	size;

	if (!str)
		return (NULL);
	head_idx = get_first_expand_sign_idx(str);
	tail_idx = get_first_expand_sign_idx(&str[head_idx + 1]);
	if (head_idx >= tail_idx)
		return (NULL);
	size = tail_idx - head_idx + 1;
	key = (char *)ft_calloc(sizeof(char), size);
	if (!key)
		return (perror_and_return_nullptr("malloc"));
	ft_strlcat(key, &str[head_idx], size + 1);
	return (key);
}


// key is not include head "$"
// $hoge
// ^start
char	*get_key_from_start_idx(const char *str, size_t start_idx)
{
	size_t	size;
	char 	*key;

	if (!str || ft_strlen_ns(str) <= start_idx + 1)
		return (NULL);
	if (is_expandable_from_start_idx(str, start_idx) == false)
		return (NULL);

	size = 1;
	if (str[start_idx + 1] != CHR_DOLLAR || str[start_idx + 1] != CHR_QUESTION) // $$ or $?
		while (str[start_idx + size] && str[start_idx + size] != CHR_DOLLAR)
			size++;
	if (str[start_idx + size] == CHR_DOLLAR)
		size--;
	key = ft_substr(str, start_idx + 1, size);
	if (!key)
		return ((char *)perror_and_return_nullptr("malloc"));
//	printf("##get_key str[start]:%s, key:%s\n", &str[start_idx], key);
	return (key);
}
// str="$"        -> "$", size=1
//     "$SHELL"   -> "/bin/bash", size=9
//     "$nothing" -> "", size=0
//     "$?"       -> "0", size=len(itoa(status))
//     "$$"       -> "4242", size=len(pid)

//TODO: Refactor, too long
size_t	get_expand_size(t_info *info, const char *str)
{
	size_t	size;
	size_t	idx;
	char	*expand_value;
	char 	*key;

	if (!str)
		return (0);
	size = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == CHR_DOLLAR && str[idx + 1])
		{
			// foo$bar,   $$$$
			//    ^ idx   ^

			if (str[idx + 1] == CHR_QUESTION || str[idx + 1] == CHR_DOLLAR) // $?
			{
				if (str[idx + 1] == CHR_QUESTION)
					expand_value = ft_itoa(info->exit_status);
				else
					expand_value = ft_itoa(info->pid);
				if (!expand_value)
					return (perror_and_return_int("malloc", FAILURE));
				size += ft_strlen_ns(expand_value);
				idx += 2; //$$ or $?
				free_1d_array_ret_nullptr((void **)&expand_value);
			}
			else
			{
				// $hoge$foo
				// ^^^^^ key now needed
				key = get_key_from_start_idx(str, idx);
				if (!key)
					return (perror_and_return_int("malloc", FAILURE));
				expand_value = get_env_value(key, info->env_list);
				size += ft_strlen_ns(expand_value);
				idx += (1 + ft_strlen_ns(key)); //$hoge
				free_1d_array_ret_nullptr((void **)&key);
			}
			continue ;
		}
		size++;
		idx++;
	}
	return (size);
}

//TODO:refactor, too long
int	expand_variable_in_str(t_info *info, char **str)
{
	char	*new_str;
	size_t	size;
	size_t	head_idx;
	size_t	var_size;
	char 	*key;
	char 	*value;

	if (!str || !*str)
		return (FAILURE);
	size = get_expand_size(info, *str);
	new_str = (char *)ft_calloc(sizeof(char), size + 1);
	if (!new_str)
		return (perror_and_return_int("malloc", FAILURE));

	// hoge$hugahugahuga$piyopiyo
	//     ^ head       ^ tail
	head_idx = 0;
	while ((*str)[head_idx])
	{
		if (is_expandable_from_start_idx(*str, head_idx))
		{
			// $$, $? or $hoge
			if ((*str)[head_idx + 1] == CHR_DOLLAR || (*str)[head_idx + 1] == CHR_QUESTION)
			{
				if ((*str)[head_idx + 1] == CHR_DOLLAR)
					value = ft_itoa(info->pid);
				else
					value = ft_itoa(info->exit_status);
				if (!value)
				{
					free_1d_array_ret_nullptr((void **)&new_str);
					return (perror_and_return_int("malloc", FAILURE));
				}
				strlcat(new_str, value, size + 1);
				free_1d_array_ret_nullptr((void **)&value);
				head_idx += 2;
			}
			else
			{
				key = get_key_from_start_idx(*str, head_idx);
				if (!key)
				{
					free_1d_array_ret_nullptr((void **)&new_str);
					return (perror_and_return_int("malloc", FAILURE));
				}
				value = get_env_value(key, info->env_list);
//				printf("key:%s, value:%s\n", key, value);
				ft_strlcat(new_str, value, size + 1);
				head_idx += (ft_strlen_ns(key) + 1);
				free_1d_array_ret_nullptr((void **)&key);
			}
		}
		else
		{
			var_size = get_first_expand_sign_idx(&(*str)[head_idx]);
			value = ft_substr(*str, head_idx, var_size);
			if (!value)
			{
				free_1d_array_ret_nullptr((void **)&new_str);
				return (perror_and_return_int("malloc", FAILURE));
			}
			ft_strlcat(new_str, value, size + 1);
			head_idx += var_size;
			free_1d_array_ret_nullptr((void **)&value);
		}
	}
	free_1d_array_ret_nullptr((void **)&(*str));
	*str = new_str;
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
			// cmds: {"echo", "hello", "$?", "$hoge$USER$nothing", NULL}q
			//    -> {"echo", "hello", "0", "piyopiyoUSERNAME", NULL}
			while (node->cmds[idx])
			{
				if (is_expand_needed(node->cmds[idx]))
				{
					if (expand_variable_in_str(info, &node->cmds[idx]) == FAILURE)
						return (FAILURE);
				}
				idx++;
			}
		}
		node = node->next;
	}
//	debug_print_stack(info->tree_root, "check tree after expansion");
	return (SUCCESS);
}
