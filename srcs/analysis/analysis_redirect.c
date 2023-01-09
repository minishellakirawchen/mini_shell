/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 08:39:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 11:04:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//  * 結合2 "infile>", "<outfile", "<<infile>" ※cmds[i]単独で判別できず前後もチェックする必要あり
// すべて"space"で結合してながら< > 前後にspaceがなかったら挿入 -> space splitする...?
// 1. 結合 & split
// 2. cmds[i] == "<", "<<", ">", ">>" を探し、file name(cmds[i++])と合わせて記録
// 3. cmds[i] != "<", "<<", ">", ">>" はchar **cmds_except_redirectに保存
// 4. cmds_except_redirectをcmdsに上書きする
// 5. コマンド実行時にin, outがredirectなら実行する


// 実行時に困ったらerrorにしよう
int	valid_redirection(char **cmds)
{
	size_t	idx;
	bool	is_error;

	idx = 0;
	is_error = false;
	while (cmds[idx])
	{
		if (cmds[idx][0] == '<' || cmds[idx][0] == '>')
		{
			if (ft_strlen_ns(cmds[idx]) > 2 || !cmds[idx + 1])
				is_error = true;
			// $> <>file はOKだが、めんどいので一旦errorにする
			// 考慮する際には次の条件をコメントアウト、さらに次の条件を有効にすればOK
			if ((cmds[idx][0] == '<' || cmds[idx][0] == '>') && (cmds[idx + 1] && (cmds[idx + 1][0] == '<' || cmds[idx + 1][0] == '>')))
				is_error = true;
//			if (cmds[idx][0] == '<' && (cmds[idx + 1] && cmds[idx + 1][0] == '>') \
//			&& (!cmds[idx + 2] || (cmds[idx + 2] && (cmds[idx + 2][0] == '<' || cmds[idx + 2][0] == '>'))))
//				is_error = true;
			if (cmds[idx][0] == '<' && (cmds[idx + 1] && cmds[idx + 1][0] == '<'))
				is_error = true;
			if (cmds[idx][0] == '>' && (cmds[idx + 1] && (cmds[idx + 1][0] == '<' || cmds[idx + 1][0] == '>')))
				is_error = true;
			if (is_error)
			{
				printf("minishell: syntax error near unexpected token `%s'\n", cmds[idx]);// TODO:STDERROR
				return (FAILURE);
			}
		}
		idx++;
	}
	return (SUCCESS);
}

// char **update = current + add
// free(current), free(add)
// TODO: more simple
char **update_files(char **current_files, char *add_file)
{
	char	**new_files;
	size_t	update_size;
	size_t	idx;

	update_size = 0;
	while (current_files && current_files[update_size])
		update_size++;
	if (add_file)
		update_size++;
	new_files = (char **)ft_calloc(sizeof(char *), update_size + 1);
	if (!new_files)
	{
		free_2d_array_ret_nullptr((void ***)&current_files);
		free_1d_array_ret_nullptr((void **)&add_file);
		return ((char **)perror_and_ret_nullptr("malloc"));
	}
	idx = 0;
	while (current_files && current_files[idx])
	{
		new_files[idx] = ft_strdup(current_files[idx]);
		if (!new_files[idx])
		{
			free_2d_array_ret_nullptr((void ***)&current_files);
			free_2d_array_ret_nullptr((void ***)&new_files);
			free_1d_array_ret_nullptr((void **)&add_file);
			return ((char **)perror_and_ret_nullptr("malloc"));
		}
		idx++;
	}
	new_files[idx] = ft_strdup(add_file);
	if (!new_files[idx])
	{
		free_2d_array_ret_nullptr((void ***)&current_files);
		free_2d_array_ret_nullptr((void ***)&new_files);
		free_1d_array_ret_nullptr((void **)&add_file);
		return ((char **)perror_and_ret_nullptr("malloc"));
	}
	free_2d_array_ret_nullptr((void ***)&current_files);
	free_1d_array_ret_nullptr((void **)&add_file);
	return (new_files);
}

void	*free_redirect_info(t_redirect_info **info)
{
	if (!info || !*info)
		return (NULL);
	free_2d_array_ret_nullptr((void ***)&(*info)->infiles);
	free_2d_array_ret_nullptr((void ***)&(*info)->outfiles);
	free_2d_array_ret_nullptr((void ***)&(*info)->here_doc_limiters);
	return (NULL);
}

int	set_redirect_in(t_redirect_info **info, char *infile)
{
	(*info)->input_from = E_REDIRECT_IN;
	(*info)->infiles = update_files((*info)->infiles, infile);
	if (!(*info)->infiles)
	{
		perror("malloc");
		free_redirect_info(&(*info));
		return (FAILURE);
	}
	return (SUCCESS);
}

int	set_redirect_out(t_redirect_info **info, char *outfile)
{
	(*info)->ouput_to = E_REDIRECT_OUT;
	(*info)->outfiles = update_files((*info)->outfiles, outfile);
	if (!(*info)->outfiles)
	{
		perror("malloc");
		free_redirect_info(&(*info));
		return (FAILURE);
	}
	return (SUCCESS);
}

int	set_redirect_heredoc(t_redirect_info **info, char *delimiter)
{
	(*info)->input_from = E_HERE_DOC;
	(*info)->here_doc_limiters = update_files((*info)->here_doc_limiters, delimiter);
	if (!(*info)->here_doc_limiters)
	{
		perror("malloc");
		free_redirect_info(&(*info));
		return (FAILURE);
	}
	return (SUCCESS);
}

//TODO: appendはoutでoverwriteできる？
int	set_redirect_append(t_redirect_info **info, char *outfile)
{
	(*info)->ouput_to = E_REDIRECT_APPEND;
	(*info)->outfiles = update_files((*info)->outfiles, outfile);
	if (!(*info)->outfiles)
	{
		perror("malloc");
		free_redirect_info(&(*info));
		return (FAILURE);
	}
	return (SUCCESS);
}

t_redirect_info	*get_redirection_info(char **cmds)
{
	size_t			idx;
	t_redirect_info	*info;

	info = (t_redirect_info *)malloc(sizeof(t_redirect_info));
	if (!info)
		return (perror_and_ret_nullptr("malloc"));
	idx = 0;
	while (cmds[idx])
	{
		if (is_same_str(cmds[idx], REDIRECT_IN))
			if (set_redirect_in(&info, cmds[idx + 1]) == FAILURE)
				return (NULL);
		if (is_same_str(cmds[idx], REDIRECT_OUT))
			if (set_redirect_out(&info, cmds[idx + 1]) == FAILURE)
				return (NULL);
		if (is_same_str(cmds[idx], REDIRECT_HEREDOC))
			if (set_redirect_heredoc(&info, cmds[idx + 1]) == FAILURE)
				return (NULL);
		if (is_same_str(cmds[idx], REDIRECT_APPEND))
			if (set_redirect_append(&info, cmds[idx + 1]) == FAILURE)
				return (NULL);
		idx++;
	}
	return (SUCCESS);
}

int	add_redirect_param(t_tree **node)
{
	char 	**splitted_cmds;

	if (!node || !*node || (*node)->exe_type != E_LEAF_COMMAND || !(*node)->cmds)
		return (FAILURE);
	// <, <<, >, >> とwordを分割する
	splitted_cmds = split_redirect_and_word_controller((const char **) (*node)->cmds);
	if (!splitted_cmds)
		return (FAILURE);
//	debug_print_2d_arr(splitted_cmds, "splitted_cmds");

	// syntax check
	// cmd[i] == redirection, cmd[i+1] == redirectionの場合はsyntax errorを出力
	if (valid_redirection(splitted_cmds) == FAILURE)
		return (SYNTAX_ERROR);

	// cmd[i] == "<", "<<", ">", ">>"の次のwordをfilename, limiterとして記録
	(*node)->redirect_info = get_redirection_info(splitted_cmds);
	if (!(*node)->redirect_info)
		return (FAILURE);

	// char **cmds_except_redirectを作成して、node->cmdsに上書き（free(node->cmds）も忘れずに）
	// free(splitted_cmds)
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




















