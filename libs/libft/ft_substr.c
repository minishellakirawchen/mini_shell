/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:40:01 by takira            #+#    #+#             */
/*   Updated: 2022/11/07 14:00:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	minsize(size_t a, size_t b)
{
	if (a <= b)
		return (a);
	return (b);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	cpy_len;
	size_t	start_idx;
	char	*ret_str;

	if (s == NULL)
		return (NULL);
	start_idx = (size_t)start;
	if (ft_strlen(s) < start_idx + 1)
		cpy_len = 0;
	else
		cpy_len = minsize(ft_strlen(s) - start_idx, len);
	ret_str = (char *)ft_calloc(sizeof(char), cpy_len + 1);
	if (ret_str == NULL)
		return (NULL);
	ft_memcpy(ret_str, &s[start_idx], cpy_len);
	return (ret_str);
}
