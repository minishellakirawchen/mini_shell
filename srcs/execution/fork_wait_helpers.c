/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_wait_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 17:44:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 17:44:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

