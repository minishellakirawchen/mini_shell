/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:01 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 20:31:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_current_path(void)
{
	char *current_path;

// TODO: implement like official
//	current_path = (char *)ft_calloc(sizeof(char), BUFSIZE);
//	while (!getcwd(current_path, BUFSIZE))
		// realloc
	current_path = getcwd(NULL, 0);
	return (current_path);
}

int ft_pwd(t_info *info)
{
	char	*current_path;

	if (!info)
		return (1); //TODO: exit?
	current_path = get_current_path();
	ft_printf("%s\n", current_path);
	return (0);
}
