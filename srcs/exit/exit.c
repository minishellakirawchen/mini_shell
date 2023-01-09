/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:45:26 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 20:44:31 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	*free_1d_2d_array_ret_nullptr(void **array1d, void ***array2d)
{
	free_1d_array_ret_nullptr(array1d);
	free_2d_array_ret_nullptr(array2d);
	return (NULL);
}


void	*free_2d_array_ret_nullptr(void ***array)
{
	size_t	i;

	if (!array || !*array)
		return (NULL);
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
	return (NULL);
}

void	*free_1d_array_ret_nullptr(void **array)
{
	if (!array)
		return (NULL);
	free(*array);
	*array = NULL;
	return (NULL);
}


void	free_info(t_info	**info)
{
	if (!info || !*info)
		return ;
	// TODO: free info members
	free(*info);
	*info = NULL;
}

int	free_and_return_no(t_info **info, int exit_status)
{
	free_info(info);
	return (exit_status);
}

char	**free_array(char **array) //TODO: ptr = NULL
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

void	*perror_and_ret_nullptr(char *err)
{
	perror(err);
	return (NULL);
}
