/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:51:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/16 13:11:36 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_execute_only_builtin(t_tree *node)
{
	return (node && node->exe_type == E_NODE_NO_PIPE && node->next \
 && node->next->cmds && is_builtins((const char **)node->next->cmds));
}

// cat | ls; lsが先に終了する(exit)のでcatの入力を待ってくれないと仮定
// grep a | lsでも終了してしまう
// cat, grep単独だと入力待ちになる
// 兄弟プロセスとしてexecを生成すると、解決する？
void	ft_execve(t_tree *node, t_info *info)
{
//	const char	*cmd;

	if (!node || !node->cmds || !info)
		exit (FAILURE);
//	cmd = node->cmds[0];
	// parent : execute RIGHT commands
//	debug_print_2d_arr(node->cmds, "cmds");
	if (handle_fd_for_redirection(node->redirect_info) == FAILURE)
		exit (EXIT_FAILURE);

	// execute builtins
	if (is_builtins((const char **)node->cmds))
		exit (execute_builtin(info, (const char **) node->cmds));//exitしないとblocking

	// execute other commands
	if (node->cmds[0] && (node->cmds[0][0] == '/' || node->cmds[0][0] == '.'))
		execve(node->cmds[0], node->cmds, NULL);
	else
		ft_execvp(node->cmds[0], node->cmds, get_env_value(PATH, info->env_list));
	ft_dprintf(STDERR_FILENO, "command not found: %s\n", node->cmds[0]);
//	ft_dprintf(STDERR_FILENO, "command not found: %s\n", cmd);
	exit (CMD_NOT_FOUND);
}
