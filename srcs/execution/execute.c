/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/13 17:40:17 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PATH: "/bin", "/bin/usr", ...
// -> /bin/ls, /bin/usr/ls, を試していく
// 成功したらok(exit), 失敗したら次のpathを試す
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
		return (perror_and_return_nullptr("malloc"));
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

	splitted_paths = ft_split(env_paths, CHA_PATH_DELIM);
	if (!splitted_paths)
		return (perror_and_return_int("malloc", EXIT_FAILURE));
//	debug_print_2d_arr(splitted_paths, "splitted_paths"); // check
	idx = 0;
	while (splitted_paths[idx])
	{
		path = get_execute_path(splitted_paths[idx], file);
		if (!path)
			return (perror_and_return_int("malloc", EXIT_FAILURE));
//		dprintf(STDERR_FILENO, "cmdpath:%s\n", path);
		execve(path, cmds, NULL);
		free(path);
		idx++;
	}
	free_array(splitted_paths);
	return (CMD_NOT_FOUND);
}

// cmd1                |            cmd2
// child                            parent
// stdout->[->write   pipe  read->]->stdin
//

int	close_fd_for_child(int pipe_fd[2])
{
	if (close(pipe_fd[READ]))
		return (perror_and_return_int("close", EXIT_FAILURE));
//	if (close(STDOUT_FILENO))
//		return (perror_and_return_int("close", EXIT_FAILURE));
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
//	if (close(STDIN_FILENO))
//		return (perror_and_return_int("close", EXIT_FAILURE));
	if (dup2(pipe_fd[READ], STDIN_FILENO) < 0)
		return (perror_and_return_int("dup2", EXIT_FAILURE));
	if (close(pipe_fd[READ]) < 0)
		return (perror_and_return_int("close", EXIT_FAILURE));
	return (SUCCESS);
}

// if redirect in/out/here_doc, handle fds
int	handle_fd_for_redirection(t_redirect_info *r_info)
{
	int	fd;

	if (!r_info)
		return (FAILURE);
	if (r_info->input_from == E_STDIN && r_info->ouput_to == E_STDOUT)
		return (SUCCESS);
	if (r_info->ouput_to == E_REDIRECT_OUT || r_info->ouput_to == E_REDIRECT_APPEND)
	{
		if (dup2(r_info->r_fd[R_FD_OUTFILE], STDOUT_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(r_info->r_fd[R_FD_OUTFILE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	if (r_info->input_from == E_REDIRECT_IN || r_info->input_from == E_HERE_DOC)
	{
		fd = R_FD_INFILE;
		if (r_info->input_from == E_HERE_DOC)
			fd = R_FD_HEREDOC;
		if (dup2(r_info->r_fd[fd], STDIN_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(r_info->r_fd[fd]) < 0)
			return (perror_and_return_int("close", FAILURE));
//		printf("handler heredoc_file:%s\n", r_info->heredoc_file);
		if (r_info->input_from == E_HERE_DOC && unlink(r_info->heredoc_file) < 0)
			return (perror_and_return_int("unlink", FAILURE));
	}
	return (SUCCESS);
}

//  <- Prev             next ->
// cmd1 | cmd2 |..|cmdn-1| cmdn
// childn   ...    child2  child1
//   out->in  ->       out->in
int execute_pipe_recursion(t_tree *node, t_info *info, int pipe_fd[2])
{
//	extern char	**environ;// env更新必要？envが必要なcommandが実行されなければexecve(hoge, hoge, NULL)で良い getenvくらい？
	pid_t		pid;

	if (!node || !node->cmds)
		exit (EXIT_FAILURE);

	if (node->prev && node->prev->exe_type == E_LEAF_COMMAND)
	{
//		pipe(pipe_fd);
		pid = fork();
		if (pid == CHILD_PROCESS)
		{
			// child : execute LEFT commands
			if (close_fd_for_child(pipe_fd))
				exit (EXIT_FAILURE);
			execute_pipe_recursion(node->prev, info, pipe_fd);
			exit (CMD_NOT_FOUND);
		}
		// parent : execute RIGHT commands
		if (close_fd_for_parent(pipe_fd))
			exit (EXIT_FAILURE);
	}

	// parent : execute RIGHT commands
	debug_print_2d_arr(node->cmds, "cmds");

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
	dprintf(STDERR_FILENO, "command not found: %s\n", node->cmds[0]);//TODO:stderr
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

int	is_execute_only_builtin(t_tree *node)
{
	return (node && node->exe_type == E_NODE_NO_PIPE && node->next \
 && node->next->cmds && is_builtins((const char **)node->next->cmds));
}

// cat | ls; lsが先に終了する(exit)のでcatの入力を待ってくれないと仮定
// grep a | lsでも終了してしまう
// cat, grep単独だと入力待ちになる
// 兄弟プロセスとしてexecを生成すると、解決する？

void	control_fds(size_t idx, int ***pipe_fds, t_tree *next)
{
	if (idx == 0)
	{
		close((*pipe_fds)[idx][READ]);
		dup2((*pipe_fds)[idx][WRITE], STDOUT_FILENO);
		close((*pipe_fds)[idx][WRITE]);
	}
	else if (next == NULL)
	{
		close((*pipe_fds)[idx - 1][WRITE]);
		dup2((*pipe_fds)[idx - 1][READ], STDIN_FILENO);
		close((*pipe_fds)[idx - 1][READ]);
	}
	else
	{
		close((*pipe_fds)[idx - 1][WRITE]);
		dup2((*pipe_fds)[idx - 1][READ], STDIN_FILENO);
		close((*pipe_fds)[idx - 1][READ]);

		close((*pipe_fds)[idx][READ]);
		dup2((*pipe_fds)[idx][WRITE], STDOUT_FILENO);
		close((*pipe_fds)[idx][WRITE]);
	}
}

// idx   0           1           2
// echo hello | cat Makefile | hoge
//         [pipe]          [pipe]
//     out->    ->in    out->   ->in

// childで実行, parentで回収？
//        idx   0               1           2
//       echo hello    | cat Makefile | hoge
// child     v stdout     ^ in
//         [pipe] w/r     ^
// parent    v stdin      ^
//           --------------

int execute_pipe_loop(t_tree *root, t_info *info)
{
	t_tree			*node;
	const size_t	node_cnt = get_tree_size(root);
	int				pipe_fds[node_cnt][2];
	pid_t			pids[node_cnt];
	size_t			idx;
	size_t			i;
	int				status;

	node = root;
	while (node->exe_type != E_LEAF_COMMAND)
		node = node->next;
	idx = 0;
	while (node)
	{
		if (node->next)
			pipe(pipe_fds[idx]);
		pids[idx] = fork();
		if (pids[idx] == 0) // child
		{
			// read from pipe
			if (node->next)
			{
				if (idx > 0)
				{
					if (close(pipe_fds[idx - 1][WRITE]) < 0)
						perror("close");
					if (close(STDIN_FILENO) < 0)
						perror("close");
					if (dup2(pipe_fds[idx - 1][READ], STDIN_FILENO) < 0)
						perror("dup2");
					if (close(pipe_fds[idx - 1][READ]) < 0)
						perror("close");
				}
				// write to pipe
				if (close(pipe_fds[idx][READ]) < 0)
					perror("close");
				if (close(STDOUT_FILENO) < 0)
					perror("close");
				if (dup2(pipe_fds[idx][WRITE], STDOUT_FILENO) < 0)
					perror("dup2");
				if (close(pipe_fds[idx][WRITE]) < 0)
					perror("close");
			}
//			control_fds(idx, (int ***)&pipe_fds, node->next);
			debug_print_2d_arr(node->cmds, "cmds");

			// redirection
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
			dprintf(STDERR_FILENO, "command not found: %s\n", node->cmds[0]);//TODO:stderr
			return (0);
			exit (CMD_NOT_FOUND);
		}
		if (node->next)
		{
			if (close(pipe_fds[idx][WRITE]) < 0)
				perror("close");
			if (close(STDIN_FILENO) < 0)
				perror("close");
			if (dup2(pipe_fds[idx][READ], STDIN_FILENO) < 0)
				perror("dup2");
			if (close(pipe_fds[idx][READ]) < 0)
				perror("close");
		}
//		close(pipe_fds[idx][READ]);
//		if (idx > 0)
//		{
//			close(pipe_fds[idx - 1][WRITE]);
//			close(pipe_fds[idx - 1][READ]);
//		}

		node = node->next;
		idx++;
	}
	i = 0;
	while (i < idx)
	{
		waitpid(pids[i++], &status, 0);
//		wait(NULL);
//		i++;
	}
	return (WEXITSTATUS(status));
}



int execute_pipe_loop_reverse(t_tree *root, t_info *info)
{
	t_tree			*node;
	const size_t	node_cnt = get_tree_size(root);
	int				pipe_fds[node_cnt][2];
	pid_t			pids[node_cnt];
	size_t			idx;
	size_t			i;
	int				status;

	node = root;
	while (node->next)
		node = node->next;

	// idx  n    n-1             0
	//   cmd0 <- cmd1 <- .., <- cmdn 再起的に実行していく
	idx = 0;
	dprintf(STDERR_FILENO, "1\n");

	// pipeを作っておく
	while (node->prev->exe_type == E_LEAF_COMMAND)
	{
		if (pipe(pipe_fds[idx]) < 0)
			perror("pipe");
		node = node->prev;
	}

	node = root;
	while (node->next)
		node = node->next;
	while (node->exe_type == E_LEAF_COMMAND)
	{
		pids[idx] = fork();
		dprintf(STDERR_FILENO, "2 pid:%d\n", pids[idx]);
		if (pids[idx] == 0) // child
		{
			// read from pipe
//			if (node->prev)
//			{
//				close(pipe_fds[idx + 1][WRITE]);
//				close(STDIN_FILENO);
//				dup2(pipe_fds[idx + 1][READ], STDIN_FILENO);
//				close(pipe_fds[idx + 1][READ]);
//			}
			if (node->prev->exe_type == E_LEAF_COMMAND)
			{
				if (close(pipe_fds[idx + 1][WRITE]) < 0)
					perror("close");
				if (close(STDIN_FILENO) < 0)
					perror("close");
				if (dup2(pipe_fds[idx + 1][READ], STDIN_FILENO) < 0)
					perror("dup2");
				if (close(pipe_fds[idx][READ]) < 0)
					perror("close");

				if (close(pipe_fds[idx][READ]) < 0)
					perror("close");
				if (close(STDOUT_FILENO) < 0)
					perror("close");
				if (dup2(pipe_fds[idx][WRITE], STDOUT_FILENO) < 0)
					perror("dup2");
				if (close(pipe_fds[idx][WRITE]) < 0)
					perror("close");
			}
			dprintf(STDERR_FILENO, "3 cmd:%s\n", node->cmds[0]);

//			control_fds(idx, (int ***)&pipe_fds, node->next);
			debug_print_2d_arr(node->cmds, "cmds");


			// redirection
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
			dprintf(STDERR_FILENO, "command not found: %s\n", node->cmds[0]);//TODO:stderr
			exit (CMD_NOT_FOUND);
		}
		if (node->prev->exe_type == E_LEAF_COMMAND)
		{
			if (close(pipe_fds[idx][WRITE]) < 0)
				perror("close");
			if (close(STDIN_FILENO) < 0)
				perror("close");
			if (dup2(pipe_fds[idx][READ], STDIN_FILENO) < 0)
				perror("dup2");
			if (close(pipe_fds[idx][READ]) < 0)
				perror("close");
		}
//		close(pipe_fds[idx][READ]);
//		if (idx > 0)
//		{
//			close(pipe_fds[idx - 1][WRITE]);
//			close(pipe_fds[idx - 1][READ]);
//		}

		node = node->prev;
		idx++;
	}
	i = 0;
	while (i < idx)
	{
		waitpid(pids[i++], &status, 0);
//		wait(NULL);
//		i++;
	}
	return (WEXITSTATUS(status));
}

void	ft_execve(t_tree *node, t_info *info)
{
	// parent : execute RIGHT commands
	debug_print_2d_arr(node->cmds, "cmds");

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
	dprintf(STDERR_FILENO, "command not found: %s\n", node->cmds[0]);//TODO:stderr
	exit (CMD_NOT_FOUND);
}



// 23/1/13
// cmd0   |   cmd1   |   cmd2   |...|   cmdn
//     [pipe]      [pipe]         [pipe]
//

// Main             ↓ stdin
//   |-- fork --> cmd0
//   |             ||pipe
//   |-- fork --> cmd1
//   |             ||pipe
//   |-- fork --> cmd2
//   |             ||pipe
//   |-- fork --> cmd3
//                  ↓ stdout

void init_pipe(int **pipe)
{
	if (!pipe || !*pipe)
		return ;
	*pipe[READ] = STDIN_FILENO;
	*pipe[WRITE] = STDOUT_FILENO;
}

int execute_pipe_line(t_info *info, t_tree *cmd_leaf_head)
{
	int				pipe_now_fd[2];
	int 			pipe_old_fd[2];
	t_tree			*node;
	int				status;

	if (!info || !cmd_leaf_head)
		return (FAILURE);
	node = cmd_leaf_head;
	pipe_old_fd[READ] = STDIN_FILENO;
	pipe_old_fd[WRITE] = -1;

	dprintf(STDERR_FILENO, "execute_pipe_line\n");
	while (node)
	{
		pipe(pipe_now_fd);

		node->pid = fork();
		if (node->pid == 0)
		{
			if (pipe_old_fd[WRITE] >= 0)
			{
				close(pipe_old_fd[WRITE]);
				dup2(pipe_old_fd[READ], STDIN_FILENO);
				close(pipe_old_fd[READ]);
			}
			if (node->next)
			{
				close(pipe_now_fd[READ]);
				dup2(pipe_now_fd[WRITE], STDOUT_FILENO);
				close(pipe_now_fd[WRITE]);
			}

			ft_execve(node, info);
			exit (CMD_NOT_FOUND);
		}
		if (pipe_old_fd[WRITE] >= 0)
		{
			close(pipe_old_fd[READ]);
			close(pipe_old_fd[WRITE]);
		}
		pipe_old_fd[READ] = pipe_now_fd[READ];
		pipe_old_fd[WRITE] = pipe_now_fd[WRITE];

		node = node->next;
	}
	dup2(pipe_old_fd[READ], STDIN_FILENO);
	close(pipe_old_fd[READ]);
	close(pipe_old_fd[WRITE]);
	close(pipe_now_fd[READ]);
	close(pipe_now_fd[WRITE]);

	node = cmd_leaf_head;
	while (node)
	{
		waitpid(node->pid, &status, 0);
		node = node->next;
	}
	return (WEXITSTATUS(status));
}


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

//	int	pipe_fd[2];
//	pipe(pipe_fd);
	pid = fork();
	status = 0;
	if (pid == CHILD_PROCESS)
	{
		status = execute_pipe_line(info, node);
		exit (0);
	}

//	close(pipe_fd[READ]);
//	close(pipe_fd[WRITE]);

	//TODO: check wait operation
	//childの終了を待ってからゾンビプロセスを全て回収
	waitpid(pid, &status, 0);
//	while (wait(NULL) > 0);
//	dprintf(STDERR_FILENO, "status:%d\n", WEXITSTATUS(status));
//	waitpid(pid, &status, 0);

	dprintf(STDERR_FILENO, "status:%d\n", status);
	return (status);
}

