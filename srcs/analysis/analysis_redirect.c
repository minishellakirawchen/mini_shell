/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analysis_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 08:39:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 08:39:47 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// (*node)->cmds = {"<", "file", "cmd0", "cmd1", ..., NULL}
// (*node)->cmds = {"<file", "cmd0", "cmd1", ..., NULL}
// if cmds[i][0]="<"
//   len(cmd[i])==1 -> cmd[i+1]  =file
//   len(cmd[i])>1  -> cmd[i][1:]=file

//TODO: <の数を調べて、infileをstockすれば良いかと思ったが
// <,<<,>,>>混在しているケースに対応できない、要再検討
size_t	get_redirect_cnt(char **cmds)
{
	size_t	cnt;
	size_t	i;
	size_t	j;

	if (!cmds || !*cmds)
		return (0);
	cnt = 0;
	i = 0;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i][j])
		{

		}
	}
}

void	add_redirect_param(t_tree **node)
{
	size_t	idx;
	char 	*cmd;
	size_t	redirect_cnt;

	if (!node || !*node || (*node)->exe_type != E_LEAF_COMMAND || !(*node)->cmds)
		return ;
	idx = 0;
	while ((*node)->cmds[idx])
	{
		cmd = (*node)->cmds[idx];
		if (cmd[0] == '<')
		{
			if (ft_strlen_ns(cmd) == 1)


		}
		idx++;
	}

}
