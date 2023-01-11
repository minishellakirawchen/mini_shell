/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_chr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:26:42 by takira            #+#    #+#             */
/*   Updated: 2023/01/10 11:26:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prototype declaration */
static char		**split_pipe_and_word(const char *src, size_t size);
static size_t	get_split_set_size(const char *str,  const char *set);

// input [  | echo hello | cat Makefile || echo "foo | bar" |]
// split {|,echo,hello,|,cat,Makefile,||,echo "foo | bar",|, NULL}

/* Function */
char **split_pipe_and_word_controller(const char *readline_input)
{
	char	**splitted_cmds;
	size_t	size;

	size = get_split_set_size(readline_input, SET_CHARS);
	splitted_cmds = split_pipe_and_word(readline_input, size);
	if (!splitted_cmds)
		return (NULL);
	return (splitted_cmds);
}

// test in playground/test_split_size.c
static size_t	get_split_set_size(const char *str,  const char *set)
{
	size_t	cnt;
	size_t	idx;
	char	*setchr;

	cnt = 0;
	if (!str)
		return (0);
	idx = 0;
	while (str[idx])
	{
		// delim(space)抜けるまでインクリメント
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^                                    ^
		while (str[idx] && ft_isspace(str[idx]))
			idx++;

		// set in str[idx]
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^                                    ^ set="
		if (str[idx] && ft_strchr(set, str[idx]))
		{
			cnt++;
			setchr = ft_strchr(set, str[idx]);
			idx++;
			// setを抜けるまでインクリメント
			// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
			//    ^                                            ^
			while (str[idx] && str[idx] != setchr[0])
				idx++;
			if (str[idx] == setchr[0])
				idx++;
		}
		// space or setならばcontinue
		if (str[idx] && (ft_isspace(str[idx]) || ft_strchr(set, str[idx])))
			continue ;

		// space or setを抜けた後, 文字列があればcnt++
		// "foo"bar => A["foo"bar] or B["foo", bar] どっち？-> A
		// 		bash-3.2$ echo "hello" world //hello world
		// 		bash-3.2$ echo "hello"world // helloworld
		// この処理ではset文字("")前後で区切る＆set文字を残し、expantionで処理する TODO:expantion

		// space or setを抜けた後, !str[idx]でなければcnt++
		if (str[idx])
			cnt++;

		// pipeならpipeを抜けるまでインクリメント
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^
		if (str[idx] && str[idx] == CHR_PIPE)
		{
			while (str[idx] && str[idx] == CHR_PIPE)
				idx++;
			continue ; //space, setの確認から実行
		}
		// pipeでない場合
		// 次のpipe or space or setまでインクリメント
		if (str[idx] && str[idx] != CHR_PIPE)
		{
			while (str[idx] && str[idx] != CHR_PIPE && !ft_isspace(str[idx]) && !ft_strchr(set, str[idx]))
				idx++;
			continue ;
		}
	}
	return (cnt);
}

// str[0] is not space
static size_t	get_word_size(const char *str,  const char *set)
{
	size_t	idx;
	char	*setchr;

	if (!str)
		return (0);
	idx = 0;
	if (str[idx] && ft_strchr(set, str[idx]))
	{
		setchr = ft_strchr(set, str[idx]);
		idx++;
		while (str[idx] && str[idx] != setchr[0])
			idx++;
		if (str[idx] == setchr[0])
			idx++;
		return (idx);
	}
	if (str[idx] && str[idx] == '|')
	{
		while (str[idx] && str[idx] == '|')
			idx++;
		return (idx);
	}
	while (str[idx] && str[idx] != '|' && !ft_isspace(str[idx]) && !ft_strchr(set, str[idx]))
		idx++;
	return (idx);
}

static char	**split_pipe_and_word(const char *src, size_t size)
{
	char	**splitted_cmds;
	size_t	cmds_idx;
	size_t	head_idx;
	size_t	word_size;

	splitted_cmds = (char **)ft_calloc(sizeof(char *), size + 1);
	if (!splitted_cmds)
		return ((char **) perror_and_return_nullptr("malloc"));
	cmds_idx = 0;
	head_idx = 0;
	while (cmds_idx < size)
	{
		while (src[head_idx] && ft_isspace(src[head_idx]))
			head_idx++;
		word_size = get_word_size(&src[head_idx], SET_CHARS);
		splitted_cmds[cmds_idx] = ft_substr(src, head_idx, word_size);
		if (!splitted_cmds[cmds_idx])
		{
			free_2d_array_ret_nullptr((void ***)&splitted_cmds);
			return ((char **) perror_and_return_nullptr("malloc"));
		}
		head_idx += word_size;
		cmds_idx++;
	}
	return (splitted_cmds);
}
