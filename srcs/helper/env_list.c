/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:25:02 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 19:49:58 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// search key-value pairs
// if there is a key matches search key, return its value
// if not found, return NULL
char *get_env_value(const char *search_key, t_list *env_list_head)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;

	if (search_key_len == 0)
		return ("");
	while (env_list_head)
	{
		elem = env_list_head->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
			return (elem->value);
		env_list_head = env_list_head->next;
	}
	return ("");
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
			free_1d_array_ret_nullptr((void **)&elem->value);
			free_1d_array_ret_nullptr((void **)&elem->key);
			free_1d_array_ret_nullptr((void **)&elem);
			if (prev)
				prev->next = ptr->next;
			free_1d_array_ret_nullptr((void **)&ptr);
			return (SUCCESS);
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return (FAILURE);
}

// overwrite value use strdup
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
			free_1d_array_ret_nullptr((void **)&elem->value);
			elem->value = ft_strdup(value);
			if (!elem->value)
				return (perror_and_return_int("malloc", FAILURE));
			return (SUCCESS);
		}
		ptr = ptr->next;
	}
	return (FAILURE);
}

char *create_concat_str(char *base_str, char *cat_str)
{
	char	*concat_str;
	size_t	concat_len;

	errno = 0;
	concat_len = ft_strlen_ns(base_str) + ft_strlen_ns(cat_str);
	concat_str = (char *)ft_calloc(sizeof(char), concat_len + 1);
	if (!concat_str)
		return (NULL);
	ft_strlcat(concat_str, base_str, concat_len + 1);
	ft_strlcat(concat_str, cat_str, concat_len + 1);
	return (concat_str);
}

// hoge=foo
// append_env_value(list, hoge, bar)
// hoge=foobar
int	append_env_value(t_list **list_head, char *search_key, char *append_value)
{
	const size_t	search_key_len = ft_strlen_ns(search_key);
	t_env_elem		*elem;
	t_list			*ptr;
	char 			*new_value;

	errno = 0;
	if (search_key_len == 0 || !list_head)
		return (FAILURE);
	ptr = *list_head;
	while (ptr)
	{
		elem = ptr->content;
		if ((ft_strlen_ns(elem->key) == search_key_len) \
		&& (ft_strncmp_ns(elem->key, search_key, search_key_len) == 0))
		{
			new_value = create_concat_str(elem->value, append_value);
			if (!new_value)
				return (perror_and_return_int("malloc", FAILURE));
			free_1d_array_ret_nullptr((void **)&elem->value);
			elem->value = ft_strdup(new_value);
			if (!elem->value)
				return (perror_and_return_int("malloc", FAILURE));
			free_1d_array_ret_nullptr((void **)&new_value);
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

	errno = 0;
	env_elem = (t_env_elem *)malloc(sizeof(t_env_elem));
	if (!env_elem)
		return (perror_and_return_int("malloc", FAILURE));
	env_elem->key = ft_strdup(key);
	env_elem->value = ft_strdup(value);
	if (!env_elem->key || !env_elem->value)
	{
		perror("malloc"); //error and exit
		free_1d_array_ret_nullptr((void **)&env_elem->key);
		free_1d_array_ret_nullptr((void **)&env_elem->value);
		free_1d_array_ret_nullptr((void **)&env_elem);
		return (FAILURE);
	}
	new_node = ft_lstnew(env_elem);
	if (!new_node)
		return (FAILURE); // TODO:free & return NULL
	ft_lstadd_back(list_head, new_node);
	return (SUCCESS);
}
