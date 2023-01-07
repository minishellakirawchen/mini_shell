/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:58:03 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 20:38:23 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: Add function update *node->cmds, delete redirect chars

t_tree	*create_tree_node(t_exe_type type, char *raw_cmd_str)
{
	t_tree		*new_node;

	new_node= (t_tree *)malloc(sizeof(t_tree));
	if (!new_node)
		return (perror_and_return_null("malloc"));
	new_node->exe_type = type;
	new_node->redirect_in = false;
	new_node->redirect_out = false;
	new_node->redirect_files = NULL;
	if (!raw_cmd_str)
	{
		new_node->cmds = NULL;
		return (new_node);
	}
	new_node->cmds = splitset_and_trim(raw_cmd_str, ' ', '"', ISSPACE);
	if (!new_node->cmds)
	{
		free(new_node);
		return (perror_and_return_null("malloc"));
	}
	return (new_node);
}

char **splitset_and_trim(char *src, char delim, char set, char *trimchar)
{
	char	**splitted_strs;
	char	*trimmed_str;
	size_t	i;

	splitted_strs = ft_split_set(src, delim, set);
	if (!splitted_strs)
		return (NULL);
	i = 0;
	while (splitted_strs[i])
	{
		trimmed_str = ft_strtrim(splitted_strs[i], trimchar);
		if (!trimmed_str)
			return (free_array(splitted_strs));
		free(splitted_strs[i]);
		splitted_strs[i] = ft_strdup(trimmed_str);
		free(trimmed_str);
		i++;
	}
//	debug_print_2d_arr(splitted_strs, "splited_str[i]");
	return (splitted_strs);
}

t_tree	*create_tree_elem(t_exe_type type, char ***cmds)
{
	t_tree	*new_elem;

	new_elem = (t_tree *)malloc(sizeof(t_tree));
	if (!new_elem)
		return (NULL);
	new_elem->exe_type = type;
	new_elem->cmds = *cmds;
	new_elem->left = NULL;
	new_elem->right = NULL;
	return (new_elem);
}
