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

// search key-value pairs
// if there is a key matches search key, return its value
// if not found, return NULL
char *get_env_value(char *search_key, t_list *env_list_head)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;

	if (search_key_len == 0)
		return (NULL);
	while (env_list_head)
	{
		elem = env_list_head->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
			return (elem->value);
		env_list_head = env_list_head->next;
	}
	return (NULL);
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

int	delete_env_elem(t_list **list_head, char *search_key)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;
	t_list			*ptr;
	t_list			*prev;

	if (search_key_len == 0 || !list_head)
		return (FAILURE);
	ptr = *list_head;
	prev = NULL;
	while (ptr)
	{
		elem = ptr->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
		{
			free(elem->value);
			free(elem->key);
			free(elem);
			if (prev)
				prev->next = ptr->next;
			free(ptr);
			return (SUCCESS);
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return (FAILURE);
}

int	overwrite_env_value(t_list **list_head, char *search_key, char *value)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;
	t_list			*ptr;

	if (search_key_len == 0 || !list_head)
		return (FAILURE);
	ptr = *list_head;
	while (ptr)
	{
		elem = ptr->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
		{
			free(elem->value);
			elem->value = ft_strdup(value);
			if (!elem->value)
			{
				perror("malloc");
				return (FAILURE);
			}
			return (SUCCESS);
		}
		ptr = ptr->next;
	}
	return (FAILURE);
}

int	add_env_elem_to_list(t_list **list_head, char *key, char *value)
{
	t_list		*new_node;
	t_env_elem	*env_elem;

	env_elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!env_elem)
	{
		perror("malloc"); //error and exit
		return (FAILURE);
	}
	env_elem->key = ft_strdup(key);
	env_elem->value = ft_strdup(value);
	if (!env_elem->key || !env_elem->value)
	{
		perror("malloc"); //error and exit
		free(env_elem->key);
		free(env_elem->value);
		free(env_elem);
		return (FAILURE);
	}
	new_node = ft_lstnew(env_elem);
	if (!new_node)
		return (FAILURE); // TODO:free & return NULL
	ft_lstadd_back(list_head, new_node);
	return (SUCCESS);
}


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
		return (NULL);
	}
	elem->key = ft_substr(environ_i, 0, idx);
	elem->value = ft_substr(environ_i, idx + 1, len - idx - 1);
	if (!elem->key || !elem->value)
	{
		perror("malloc"); //error and exit
		free(elem->key);
		free(elem->value);
		free(elem);
		return (NULL);
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
	return (env_list_head);
}
