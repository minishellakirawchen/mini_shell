/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 14:51:23 by takira           ###   ########.fr       */
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
	{
		perror("malloc");
		return (NULL);
	}
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
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
//	debug_print_2d_arr(splitted_paths, "splitted_paths"); // check
	idx = 0;
	while (splitted_paths[idx])
	{
		path = get_execute_path(splitted_paths[idx], file);
		if (!path)
		{
			perror("malloc");
			return (EXIT_FAILURE);
		}
		execve(path, cmds, NULL);
		free(path);
		idx++;
	}
	free_array(splitted_paths);
	return (CMD_NOT_FOUND);
}

int execute_pipe_recursion(t_tree *right_elem, t_info *info)//tmp
{
//	extern char	**environ;
	pid_t		pid;
	int			pipe_fd[2];

	if (right_elem && right_elem->left && right_elem->left->exe_type == E_CMD)
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid == 0)
		{
			//TODO: error handling
			close(pipe_fd[READ]);
			close(STDOUT_FILENO);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			execute_pipe_recursion(right_elem->left, info);
		}
		//TODO: error handling
		close(pipe_fd[WRITE]);
		close(STDIN_FILENO);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	if (!right_elem || !right_elem->cmds)
		return (EXIT_FAILURE);
//	debug_print_2d_arr(elem->cmds, "cmds");

	if (is_builtins(right_elem->cmds[0]))
		return (execute_builtins(info, right_elem->cmds));
	if (right_elem->cmds[0] && (right_elem->cmds[0][0] == '/' || right_elem->cmds[0][0] == '.'))
		execve(right_elem->cmds[0], right_elem->cmds, NULL);
	else
	{
		ft_execvp(right_elem->cmds[0], right_elem->cmds, get_env_value(PATH, info->env_list));
		printf("command not found: %s\n", right_elem->cmds[0]);
	}
	exit (CMD_NOT_FOUND);
}

// TODO: implement handler
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

int	execute_command_line(t_info *info)
{
	pid_t	pid;
	t_tree	*pipe_last;
	size_t	i;
	int		status;

	//TODO: only command; no pipe
	//  cd     -> no fork
	//  others -> fork
	pid = fork();
	if (pid == 0)
	{
		pipe_last = get_last_elem(info->exe_root->right);
		//TODO: execute_handler()
		execute_pipe_recursion(pipe_last, info);
	}
	//TODO: wait良くわかっていない
	i = 0;
	while (i < get_tree_size(info->exe_root->right) + 1)
	{
		wait(&status);
		i++;
	}
	info->exit_status = WEXITSTATUS(status);
	return (info->exit_status);
}
