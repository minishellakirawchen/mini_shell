/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 18:41:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_execute_path(char *path, char *file)
{
	char			*execute_path;
	size_t			len;
	const size_t	path_len = ft_strlen_ns(path);
	const size_t	file_len = ft_strlen_ns(file);

	len = path_len + file_len;
	if (path_len > 0 && path[path_len - 1] != '/')
		len++;
	len++;
	execute_path = (char *)ft_calloc(sizeof(char), len);
	if (!execute_path)
		return (perror_and_ret_nullptr("malloc"));
	ft_strlcat(execute_path, path, len);
	if (path_len > 0 && path[path_len - 1] != '/')
		ft_strlcat(execute_path, "/", len);
	ft_strlcat(execute_path, file, len);
//	printf("create path:[%s]\n", execute_path); // check
	return (execute_path);
}

int	ft_execvp(char *file, char **cmds, char *env_paths)
{
//	extern char	**environ;
	char		**splitted_paths;
	size_t		idx;
	char 		*path;

	splitted_paths = ft_split(env_paths, PATH_DELIMITER);
	if (!splitted_paths)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
//	debug_print_2d_arr(splitted_paths, "splitted_paths"); // check
	idx = 0;
	while (splitted_paths[idx])
	{
		path = get_execute_path(splitted_paths[idx], file);
		if (!path)
			return (perror_and_return_int("malloc", EXIT_FAILURE));
		execve(path, cmds, NULL);
		free(path);
		idx++;
	}
	free_array(splitted_paths);
	return (CMD_NOT_FOUND);
}

int	close_fd_for_child(int pipe_fd[2])
{
	if (close(pipe_fd[READ]))
		return (perror_and_return_int("close", EXIT_FAILURE));
	if (close(STDOUT_FILENO))
		return (perror_and_return_int("close", EXIT_FAILURE));
	if (dup2(pipe_fd[WRITE], STDOUT_FILENO) < 0)
		return (perror_and_return_int("dup2", EXIT_FAILURE));
	if (close(pipe_fd[WRITE]) < 0)
		return (perror_and_return_int("close", EXIT_FAILURE));
	return (SUCCESS);
}

int	close_fd_for_parent(int pipe_fd[2])
{
	if (close(pipe_fd[WRITE]))
		return (perror_and_return_int("close", EXIT_FAILURE));
	if (close(STDIN_FILENO))
		return (perror_and_return_int("close", EXIT_FAILURE));
	if (dup2(pipe_fd[READ], STDIN_FILENO) < 0)
		return (perror_and_return_int("dup2", EXIT_FAILURE));
	if (close(pipe_fd[READ]) < 0)
		return (perror_and_return_int("close", EXIT_FAILURE));
	return (SUCCESS);
}

int	redirect_handler(t_redirect_info *redirect_info)
{
	if (!redirect_info)
		return (FAILURE);

	// fd

	return (SUCCESS);
}

// cmd1 | cmd2 |..|cmdn-1| cmdn
// childn   ...    child2  child1
//   out->in  ->       out->in
int execute_pipe_recursion(t_tree *right_elem, t_info *info)//tmp
{
//	extern char	**environ;
	pid_t		pid;
	int			pipe_fd[2];

	if (right_elem && right_elem->prev && right_elem->prev->exe_type == E_LEAF_COMMAND)
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid == CHILD_PROCESS)
		{
			if (close_fd_for_child(pipe_fd))
				exit (EXIT_FAILURE);
			// child execute prev commands
			execute_pipe_recursion(right_elem->prev, info);
		}
		if (close_fd_for_parent(pipe_fd))
			exit (EXIT_FAILURE);
	}
	if (!right_elem || !right_elem->cmds)
		return (EXIT_FAILURE);
//	debug_print_2d_arr(right_elem->cmds, "cmds");

	// parent execute next commands
	if (redirect_handler(right_elem->redirect_info) == FAILURE)
		exit (EXIT_FAILURE);

	if (is_builtins(right_elem->cmds))
		exit (execute_builtins(info, right_elem->cmds));//exitしないとblocking ??
	if (right_elem->cmds[0] && (right_elem->cmds[0][0] == '/' || right_elem->cmds[0][0] == '.'))
		execve(right_elem->cmds[0], right_elem->cmds, NULL);
	else
	{
		ft_execvp(right_elem->cmds[0], right_elem->cmds, get_env_value(PATH, info->env_list));
		printf("command not found: %s\n", right_elem->cmds[0]);
	}
	exit (CMD_NOT_FOUND);
}

// TODO: implement handler (Bonus part)
// nodeを辿りながらflagに応じた実行をしていく
// node中にflagがあった場合、内部でhandlerを実行することで、実行順の整合性が取れる（はず）
/*
int execute_handler()
{
	if pipe -> exec_pipe
	if subshell -> exec_subshell
	if and -> exec_and
	if or -> exec_or
}
*/

int	is_node_shell_and_cmd_cd(t_tree *node)
{
	return (node && node->exe_type == E_NODE_SHELL && node->next \
	&& node->next->cmds && is_same_str(node->next->cmds[0], "ft_cd"));
}

// rootから連結nodeを見ていき、各redirect_infoに応じたfd操作を実施
// open成功ならばupdate
// here_docも前から実行
// どこかで失敗（open fail, etc）ならば、コマンドは実行しない
int	do_redirection(t_tree **root)
{
	if (!root | !*root | !(*root)->next)
		return (FAILURE);

	return (SUCCESS);
}

int	execute_command_line(t_info *info)
{
	pid_t	pid;
	t_tree	*end_of_pipe_leaf;
	size_t	i;
	int		status;

	//type=shell(no pipe, only one command)
	//  cd     -> must NOT fork to reflect working dir
	//  others -> must fork     to exit execute process, but ft_builtins doesn't matter
	if (!info || !info->tree_root || !info->tree_root->next)
		return (EXIT_FAILURE);
	if (do_redirection(&info->tree_root) == FAILURE)
		return (EXIT_FAILURE);

	if (is_node_shell_and_cmd_cd(info->tree_root->next))
	{
		printf("shell cd\n");
		return (ft_cd(info, info->tree_root->next->next->cmds));
	}
	pid = fork();
	if (pid == 0)
	{
		end_of_pipe_leaf = get_last_node(info->tree_root->next);
		execute_pipe_recursion(end_of_pipe_leaf, info);
	}
	//TODO: check wait operation
	i = 0;
	while (i++ < get_tree_size(info->tree_root->next) + 1)
		wait(&status);
	return (WEXITSTATUS(status));
}
