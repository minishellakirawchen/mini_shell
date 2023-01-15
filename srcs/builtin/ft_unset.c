/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:14 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 21:52:53 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_unset(t_info *info, const char **cmds)
{
	char	*key;
	int		ret;

	if (!info || !cmds || !cmds[0] || !cmds[1])//tmp
		return (FAILURE);
	key = (char *)cmds[1]; //tmp
	if (get_env_value(key, info->env_list))
	{
		ret = delete_env_elem(&info->env_list, key);
		if (ret == FAILURE)
			return (FAILURE);//TODO:exit?
	}
//	printf("key:%s, get_val:%s\n", key, get_env_value(key, info->env_list));
	return (SUCCESS);
}
