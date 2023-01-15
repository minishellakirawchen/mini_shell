/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_iterative.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:58:06 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 22:08:29 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototype declaration */
static int	handle_fds_in_parent(int new_fd[2], int old_fd[2]);
static int	execute_command_in_child(t_info *info, t_tree *node, int new_fd[2], int old_fd[2]);
static int	wait_and_get_last_status(t_tree *cmd_leaf_head);

/* functions */

// Main             ↓ stdin
//   |-- fork --> cmd0
//   |             ||pipe
//   |-- fork --> cmd1
//   |             ||pipe
//   |-- fork --> cmd2
//   |             ||pipe
//   |-- fork --> cmd3
//                  ↓ stdout

//dprintf(STDERR_FILENO, "main  3: old:(r,w)=(%2d,%2d), now:(r,w)=(%2d,%2d)\n", old_pipe_fd[0], old_pipe_fd[1], new_pipe_fd[0], new_pipe_fd[1]);
//dprintf(STDERR_FILENO, "       : old:(r,w)=(%2zd,%2zd), now:(r,w)=(%2zd,%2zd)\n", read(old_pipe_fd[0], NULL, 0), read(old_pipe_fd[1], NULL, 0), read(new_pipe_fd[0], NULL, 0), read(new_pipe_fd[1], NULL, 0));

int execute_pipe_iterative(t_info *info, t_tree *cmd_leaf_head)
{
	int		new_pipe_fd[2];
	int 	old_pipe_fd[2];
	t_tree	*node;
	int		exit_status;

	init_signal_in_execute_pipe();
	if (!info || !cmd_leaf_head)
		return (FAILURE);
	node = cmd_leaf_head;
	init_pipe_fd(old_pipe_fd, new_pipe_fd);
	while (node)
	{
		pipe(new_pipe_fd);
		node->pid = fork();
		if (is_fork_failure(node->pid))
			perror_and_return_int("fork", FAILURE);
		if (is_child_process(node->pid))
			exit (execute_command_in_child(info, node, new_pipe_fd, old_pipe_fd));
		if (is_parent_process(node->pid))
			if (handle_fds_in_parent(new_pipe_fd, old_pipe_fd) == FAILURE)
				return (FAILURE);
		node = node->next;
	}
	if (close(new_pipe_fd[READ]) < 0 || close(new_pipe_fd[WRITE]) < 0)
		perror_and_return_int("close", FAILURE);
	exit_status = wait_and_get_last_status(cmd_leaf_head);
	return (exit_status);
}

static int	wait_and_get_last_status(t_tree *cmd_leaf_head)
{
	t_tree	*node;
	int		status;

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

static int	execute_command_in_child(t_info *info, t_tree *node, int new_fd[2], int old_fd[2])
{
	errno = 0;
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
	ft_execve(node, info);
	return (CMD_NOT_FOUND);
}

static int	handle_fds_in_parent(int new_fd[2], int old_fd[2])
{
	errno = 0;
	if (old_fd[WRITE] != -1)
		if (close(old_fd[READ]) || close(old_fd[WRITE]))
			return (perror_and_return_int("close", FAILURE));
	copy_fd_new_to_old(old_fd, new_fd);
	return (SUCCESS);
}
