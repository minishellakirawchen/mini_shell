/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:45:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 10:03:19 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	free_alloc(t_info	**info)
{
	if (!info || !*info)
		return ;
	// TODO: free info members
	free(*info);
	*info = NULL;
}

int	free_and_return_no(t_info **info, int exit_status)
{
	free_alloc(info);
	return (exit_status);
}
