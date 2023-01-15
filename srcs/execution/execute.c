/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 17:52:43 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// rootから連結nodeを見ていき、各redirect_infoに応じたfd操作を実施
// open成功ならばupdate
// here_docも前から実行
// どこかで失敗（open fail, etc）ならば、コマンドは実行しない
int	execute_command_line(t_info *info)
{
	pid_t	pid;
	int		status;
	t_tree	*node;

	if (!info || !info->tree_root || !info->tree_root->next)
		return (EXIT_FAILURE);
	if (execute_redirect(&info->tree_root) == FAILURE)
		return (EXIT_FAILURE);
	if (is_execute_only_builtin(info->tree_root->next))
		return (execute_builtin(info, (const char **) info->tree_root->next->next->cmds));
	node = info->tree_root;
	while (node && node->exe_type != E_LEAF_COMMAND)
		node = node->next;
	if (!node)
		return (FAILURE);
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		status = execute_pipe_iterative(info, node);
		exit (status);
	}
	waitpid(pid, &status, 0);
	status = WEXITSTATUS(status);
	return (status);
}

