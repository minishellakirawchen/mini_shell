/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_for_printf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 10:15:31 by takira            #+#    #+#             */
/*   Updated: 2022/11/15 10:15:32 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

ssize_t	ft_putchar_for_printf(char c, int fd)
{
	return (write(fd, &c, 1));
}

ssize_t	ft_putstr_for_printf(char *s, int fd)
{
	ssize_t	put_bytes;

	put_bytes = 0;
	if (s == NULL)
		return (0);
	while (*s)
	{
		put_bytes += ft_putchar_for_printf(*s++, fd);
		if (errno != 0)
			return (-1);
		if (put_bytes > INT_MAX)
			return (-1);
	}
	return (put_bytes);
}
