/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:18:38 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 21:38:25 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_printf_info	*init_info(int fd)
{
	t_printf_info	*info;

	errno = 0;
	info = (t_printf_info *)malloc(sizeof(t_printf_info));
	if (!info)
	{
		perror("malloc");
		return (NULL);
	}
	info->fmt_idx = 0;
	info->fd = fd;
	return (info);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	t_printf_info	*info;
	ssize_t			sum_print_bytes;

	if (!fmt || fd < 0 || OPEN_MAX < fd)
		return (-1);
	info = init_info(fd);
	if (!info)
		return (-1);
	sum_print_bytes = printf_controller(info, fmt);
	free(info);
	return (sum_print_bytes);
}

int	ft_printf(const char *fmt, ...)
{
	t_printf_info	*info;
	ssize_t			sum_print_bytes;

	if (!fmt)
		return (-1);
	info = init_info(STDOUT_FILENO);
	if (!info)
		return (-1);
	sum_print_bytes = printf_controller(info, fmt);
	free(info);
	return (sum_print_bytes);
}
