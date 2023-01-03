/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:37:48 by takira            #+#    #+#             */
/*   Updated: 2022/11/07 14:00:00 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ret_str;
	size_t	len;

	len = ft_strlen(s1);
	ret_str = (char *)ft_calloc(sizeof(char), len + 1);
	if (ret_str == NULL)
		return (NULL);
	ft_memcpy(ret_str, s1, len);
	return (ret_str);
}
