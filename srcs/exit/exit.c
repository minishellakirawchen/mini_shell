/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:45:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 13:22:58 by takira           ###   ########.fr       */
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

char	**free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
	return (NULL);
}

int	perror_and_return_int(char *err, int ret_value)
{
	perror(err);
	return (ret_value);
}
