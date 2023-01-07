/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 11:21:56 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// input ->
// TODO: input "key=value" -> separate to key, value
int ft_export(t_info *info, char **cmds)
{
	char	*key;
	char	*value;
	int		ret;

	if (!info || !cmds || !cmds[0] || !cmds[1] || !cmds[2])//tmp
		return (1);
	key = cmds[1]; //tmp
	value = cmds[2]; //tmp
//	printf("key:%s,value:%s\n", key, value);
	if (!get_env_value(key, info->env_list))
	{
		ret = add_env_elem_to_list(&info->env_list, key, value);
		if (ret == FAILURE)
			return (1);//TODO: exit?
	}
	else
	{
		ret = overwrite_env_value(&info->env_list, key, value);
		if (ret == FAILURE)
			return (1);//TODO: exit?
	}
//	printf("key:%s,get_value:%s\n", key, get_env_value(key, info->env_list));
	return (0);
}
