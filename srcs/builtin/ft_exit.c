/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:24 by takira            #+#    #+#             */
/*   Updated: 2023/01/06 18:26:27 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: print "exit"
int ft_exit(t_info *info)
{
	int	exit_status;

	exit_status = EXIT_SUCCESS;
	if (info)
		exit_status = info->exit_status;
	free_alloc(&info);
	exit_status = 10;
	exit(exit_status);
}
