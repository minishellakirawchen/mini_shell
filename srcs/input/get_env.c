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

void	print_key_value(void *content)
{
	t_env_elem	*elem;

	elem = content;
	printf(" key[%s], value:[%s]\n", elem->key, elem->value);

}

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


// '='idx
//  v  v len
// 0123
// A=B
//   ^ len-idx-1

// A= :OK
// A  :NG
static t_env_elem	*get_env_elem(const char *environ_i)
{
	t_env_elem			*elem;
	size_t				idx;
	const size_t		len = ft_strlen_ns(environ_i);

	if (!environ_i)
		return (NULL);//env=NULL
	idx = 0;
	while (environ_i[idx] && environ_i[idx] != '=')
		idx++;
	if (environ_i[idx] == '\0') //unknown variable, Errror?
		return (NULL);
	elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!elem)
	{
		perror("malloc"); //error and exit
		free_and_return_no(NULL, EXIT_FAILURE);
	}
	elem->key = ft_substr(environ_i, 0, idx);
	elem->value = ft_substr(environ_i, idx + 1, len - idx - 1);
	if (!elem->key || !elem->value)
	{
		perror("malloc"); //error and exit
		free_and_return_no(NULL, EXIT_FAILURE);
	}
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
		return (NULL);
	env_list_head = NULL;
	while (environ[idx])
	{
		env_elem = get_env_elem(environ[idx]);
		if (!env_elem)
			free_and_return_no(NULL, EXIT_FAILURE);
		new_node = ft_lstnew(env_elem);
		if (!new_node)
			return (NULL); // TODO:free & return NULL
		ft_lstadd_back(&env_list_head, new_node);
		idx++;
	}
	printf("print env_list\n");
	ft_lstiter(env_list_head, print_key_value); //test ok but uncheck all unset case
	return (env_list_head);
}
