/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 20:12:46 by takira           ###   ########.fr       */
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

	errno = 0;
	status = EXIT_SUCCESS;
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
	if (is_fork_failure(pid))
		return (perror_and_return_int("fork", FAILURE));
	if (is_child_process(pid))
	{
		status = execute_pipe_iterative(info, node);
		exit (status);
	}
	if (is_parent_process(pid))
	{
//		waitpid(pid, &status, 0);
		if (waitpid(pid, &status, 0) < 0)//TODO
			return (perror_and_return_int("waitpid", FAILURE));
		status = WEXITSTATUS(status);
	}
	return (status);
}

