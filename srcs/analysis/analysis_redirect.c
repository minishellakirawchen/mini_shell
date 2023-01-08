/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 08:39:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 19:24:32 by takira           ###   ########.fr       */
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
	// <, <<, >, >> とwordを分割する
	splitted_cmds = split_redirect_and_word_controller((const char **) (*node)->cmds);
	if (!splitted_cmds)
		return (FAILURE);
	debug_print_2d_arr(splitted_cmds, "splitted_cmds");

	// cmd[i] == "<", "<<", ">", ">>"の次のwordをfilename, limiterとして記録
	// cmd[i] == redirection, cmd[i+1] == redirectionの場合はsyntax errorを出力
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




















