/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:15:30 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 10:03:13 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// key-value
//		"SHELL" : "/bin/bash"
//		"PATH" : ""
// if cmd env
// 	  key + = + value
//		SHELL=/bin/bash
//      PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
//  or
// char **
//  	{
//  	"SHELL=/bin/bash",
// 		 "PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin",
//  	...,
//  	NULL
// 		}

static t_env_elem	*get_env_elem(const char *environ_i)
{
	t_env_elem	*elem;

	if (!environ_i)//tml
		return (NULL);
	elem = NULL;
	return (elem);
}

t_list	*get_env_list(void)
{
	extern char	**environ;
	t_list		*env_list_head;
	t_list		*new_node;
	t_env_elem	*env_elem;
	size_t		idx;

	idx = 0;
	if (!environ)
		return (NULL); // TODO:unset env->environ=NULL?
	while (environ[idx])
	{
		env_elem = get_env_elem(environ[idx]);
		// "SHELL=/bin/bash" -> key:"SHELL", val:"/bin/bash"
		if (!env_elem)
			return (NULL); // TODO:free & return NULL
		new_node = ft_lstnew(env_elem);
		if (!new_node)
			return (NULL); // TODO:free & return NULL
		ft_lstadd_back(&env_list_head, new_node);
		idx++;
	}
	return (env_list_head);
}
