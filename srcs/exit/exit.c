/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:45:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/02 21:45:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./../includes/minishell.h"

void	free_alloc(t_info	**info)
{
	if (!info || !*info)
		return ;
	free(*info);
	*info = NULL;
}

int	free_and_return_no(t_info **info, int exit_status)
{
	free_alloc(info);
	return (exit_status);
}
