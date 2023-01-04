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

int ft_pwd(t_info *info)
{
	char	*current_path;

	if (!info)
		return (1);
	current_path = getcwd(NULL, 0);
	ft_printf("%s\n", current_path);
	return (0);
}
