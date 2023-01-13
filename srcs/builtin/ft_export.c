/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:31:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/13 22:19:14 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_export_param(const char *cmd, char **key, char **value, t_export_type *type)
{
	size_t	idx;
	size_t	key_len;

	idx = 0;
	while (cmd[idx] && cmd[idx] != '=')
		idx++;
	if (!cmd[idx])
		return (FAILURE);
	if (idx == 0)
	{
		dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", (char *)cmd);
		return (FAILURE);
	}
	*type = E_VAR_CREATE;
	key_len = idx;
	if (cmd[idx - 1] == '+')
	{
		*type = E_VAR_ADD;
		key_len--;
	}
	*key = ft_substr(cmd, 0, key_len);
	if (!is_name((char *)key))
	{
		dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", (char *)cmd);
		free_1d_array_ret_nullptr((void **)key);
		return (FAILURE);
	}
	*value = ft_substr(cmd, idx + 1, ft_strlen_ns(cmd) - idx);
	if (!*key || !*value)
	{
		perror("malloc");
		free_1d_array_ret_nullptr((void **)key);
		free_1d_array_ret_nullptr((void **)value);
		return (EXIT_FAILURE);
	}
	return (SUCCESS);
}

// input: {export, key=value, hoge, hoge, ..., NULL}
int ft_export(t_info *info, const char **cmds)
{
	char			*key;
	char			*value;
	t_export_type	type;
	int				ret;

	if (!info || !cmds || !cmds[0] || !cmds[1])
		return (EXIT_FAILURE);
	if (get_export_param(cmds[1], &key, &value, &type) == FAILURE)
		return (EXIT_FAILURE); //TODO: key only, bash returns 0
	/*
	printf("key:%s,value:%s, cmd:%s, flag:", key, value, cmds[1]);
	if (type == E_VAR_CREATE)
		printf("NEW\n");
	else
		printf("ADD\n");
	*/
	if (!get_env_value(key, info->env_list))
		ret = add_env_elem_to_list(&info->env_list, key, value);
	else if (type == E_VAR_CREATE)
		ret = overwrite_env_value(&info->env_list, key, value);
	else
		ret = append_env_value(&info->env_list, key, value);

	if (ret == FAILURE)
		return (1);//TODO: exit?

//	printf("key:%s,get_value:%s\n", key, get_env_value(key, info->env_list));
	free_1d_array_ret_nullptr((void **)&key);
	free_1d_array_ret_nullptr((void **)&value);
	return (0);
}
