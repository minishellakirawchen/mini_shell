/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:41:06 by takira            #+#    #+#             */
/*   Updated: 2022/11/13 18:18:42 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dst_cpy;
	const unsigned char	*src_cpy;
	size_t				i;
	size_t				j;

	if (dst == src)
		return (dst);
	dst_cpy = (unsigned char *)dst;
	src_cpy = (const unsigned char *)src;
	i = 0;
	j = 0;
	while (i < n)
		dst_cpy[i++] = src_cpy[j++];
	return (dst);
}
