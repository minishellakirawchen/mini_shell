/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 08:23:00 by takira            #+#    #+#             */
/*   Updated: 2023/01/14 19:29:35 by takira           ###   ########.fr       */
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
/*
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
*/
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
/*
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
*/


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
void	execute_cmds(t_tree *node, t_info *info)
{
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

void init_pipe(int old_pipe_fd[2], int new_pipe_fd[2])
{
	if (!old_pipe_fd || !new_pipe_fd )
		return ;
	old_pipe_fd[READ] = STDIN_FILENO;
	old_pipe_fd[WRITE] = -1;
	new_pipe_fd[READ] = -1;
	new_pipe_fd[WRITE] = -1;
}

//dprintf(STDERR_FILENO, "main  3: old:(r,w)=(%2d,%2d), now:(r,w)=(%2d,%2d)\n", old_pipe_fd[0], old_pipe_fd[1], new_pipe_fd[0], new_pipe_fd[1]);
//dprintf(STDERR_FILENO, "       : old:(r,w)=(%2zd,%2zd), now:(r,w)=(%2zd,%2zd)\n", read(old_pipe_fd[0], NULL, 0), read(old_pipe_fd[1], NULL, 0), read(new_pipe_fd[0], NULL, 0), read(new_pipe_fd[1], NULL, 0));

void	copy_fd_new_to_old(int old_pipe_fd[2], const int new_pipe_fd[2])
{
	if (!old_pipe_fd || !new_pipe_fd)
		return ;
	old_pipe_fd[READ] = new_pipe_fd[READ];
	old_pipe_fd[WRITE] = new_pipe_fd[WRITE];
}

int	wait_and_get_last_status(t_tree *cmd_leaf_head)
{
	t_tree			*node;
	int				status;

	if (!cmd_leaf_head)
		return (FAILURE);

	node = get_last_node(cmd_leaf_head);
	waitpid(node->pid, &status, 0);

	node = cmd_leaf_head;
	while (node)
	{
		if (node->pid > 0)
			waitpid(node->pid, NULL, 0);
		node = node->next;
	}
	return (WEXITSTATUS(status));
}

int	execute_child(t_info *info, t_tree *node, int new_fd[2], int old_fd[2])
{
	if (old_fd[WRITE] != -1)
	{
		if (dup2(old_fd[READ], STDIN_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(old_fd[READ]) < 0 || close(old_fd[WRITE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	if (node->next)
	{
		if (dup2(new_fd[WRITE], STDOUT_FILENO) < 0)
			return (perror_and_return_int("dup2", FAILURE));
		if (close(new_fd[READ]) < 0 || close(new_fd[WRITE]) < 0)
			return (perror_and_return_int("close", FAILURE));
	}
	execute_cmds(node, info);
	return (CMD_NOT_FOUND);
}

bool	is_child_process(pid_t pid)
{
	return (pid == CHILD_PROCESS);
}

bool	is_parent_process(pid_t pid)
{
	return (pid > 0);
}

bool	is_fork_failure(pid_t pid)
{
	return (pid < 0);
}

int	execute_parent(int new_fd[2], int old_fd[2])
{
	if (old_fd[WRITE] != -1)
		if (close(old_fd[READ]) || close(old_fd[WRITE]))
			return (perror_and_return_int("close", FAILURE));
	copy_fd_new_to_old(old_fd, new_fd);
	return (SUCCESS);
}

int sig_flg;

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		sig_flg = SIGINT;
		printf("SIGINT: test newline, flg:%d\n", sig_flg);
	}
	if (signo == SIGQUIT)
	{
		sig_flg = SIGQUIT;
		printf("SIGQUIT: test quit, flg:%d\n", sig_flg);
	}
}


int execute_pipe_line(t_info *info, t_tree *cmd_leaf_head)
{
	int				new_pipe_fd[2];
	int 			old_pipe_fd[2];
	t_tree			*node;
	int				exit_status;

	if (!info || !cmd_leaf_head)
		return (FAILURE);
	node = cmd_leaf_head;
	init_pipe(old_pipe_fd, new_pipe_fd);
	while (node)
	{
		sig_flg = 0;

		pipe(new_pipe_fd);
		node->pid = fork();
		if (is_fork_failure(node->pid))
			perror_and_return_int("fork", FAILURE);
		if (is_child_process(node->pid))
			exit (execute_child(info, node, new_pipe_fd, old_pipe_fd));
		if (is_parent_process(node->pid))
			if (execute_parent(new_pipe_fd, old_pipe_fd) == FAILURE)
				return (FAILURE);
		if (signal(SIGQUIT, sig_handler) == SIG_ERR)
			perror("signal");
		if (sig_flg == SIGQUIT)
		{
			dprintf(STDERR_FILENO, "^\\Quit: 3\n");
			exit (EXIT_SIGQUIT);
		}
		node = node->next;
	}
	if (close(new_pipe_fd[READ]) < 0 || close(new_pipe_fd[WRITE]) < 0)
		perror_and_return_int("close", FAILURE);
	exit_status = wait_and_get_last_status(cmd_leaf_head);
	return (exit_status);
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
	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		status = execute_pipe_line(info, node);
		exit (status);
	}
	waitpid(pid, &status, 0);
	status = WEXITSTATUS(status);
	return (status);
}

