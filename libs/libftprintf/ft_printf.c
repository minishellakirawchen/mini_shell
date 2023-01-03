/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:52:04 by takira            #+#    #+#             */
/*   Updated: 2022/11/17 13:39:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	init_info_for_fmt(t_printf_info *info)
{
	info->flag_left = false;
	info->flag_sign = false;
	info->flag_space = false;
	info->flag_prefix = false;
	info->flag_zero_pad = false;
	info->prec_dot = false;
	info->prec_dot_only = false;
	info->is_pointer = false;
	info->width_siz = 0;
	info->perc_siz = 0;
	info->num_base = 10;
	info->num_capitals = 0;
	info->num_padlen = 0;
	info->num_preclen = 0;
	info->num_head_chr = "";
}

static ssize_t	pass_to_printfunc(const char c, t_printf_info info, va_list *p)
{
	if (c == 'c')
		return (print_c(va_arg(*p, int), info));
	if (c == 's')
		return (print_s(va_arg(*p, char *), info));
	if (c == 'p')
		return (print_hex(va_arg(*p, uintptr_t), &info, false));
	if (c == 'd' || c == 'i')
		return (print_signed(va_arg(*p, int), &info));
	if (c == 'u')
		return (print_unsigned(va_arg(*p, unsigned int), info));
	if (c == 'x')
		return (print_hex(va_arg(*p, unsigned int), &info, false));
	if (c == 'X')
		return (print_hex(va_arg(*p, unsigned int), &info, true));
	if (c == '%')
		return (print_percent(info));
	return (-1);
}

static ssize_t	print_fmt(char *fmt, t_printf_info *info, va_list *p)
{
	init_info_for_fmt(info);
	get_flag((char *)fmt, info);
	if (errno != 0)
		return (-1);
	if (get_width((char *) fmt, info, p) == FAIL)
		return (-1);
	if (get_prec((char *) fmt, info, p) == FAIL)
		return (-1);
	if (valid_info4fmt(fmt[info->fmt_idx], info) == FAIL)
		return (-1);
	return (pass_to_printfunc(fmt[info->fmt_idx], *info, p));
}

static void	init_printf_param(size_t *fmt_idx, ssize_t *bytes)
{
	*bytes = 0;
	*fmt_idx = 0;
	errno = 0;
}

int	ft_printf(const char *fmt, ...)
{
	va_list			ptr;
	t_printf_info	info;
	ssize_t			print_bytes;
	ssize_t			sum_print_bytes;

	if (!fmt)
		return (-1);
	va_start(ptr, fmt);
	init_printf_param(&info.fmt_idx, &sum_print_bytes);
	while (fmt[info.fmt_idx] && errno == 0 && sum_print_bytes >= 0)
	{
		while (fmt[info.fmt_idx] != '%' && fmt[info.fmt_idx] && errno == 0)
			sum_print_bytes += ft_putchar_for_printf(fmt[info.fmt_idx++], 1);
		if (!fmt[info.fmt_idx])
			break ;
		info.fmt_idx++;
		print_bytes = print_fmt((char *)fmt, &info, &ptr);
		sum_print_bytes += print_bytes;
		if (errno != 0 || print_bytes == -1 || sum_print_bytes > INT_MAX)
			sum_print_bytes = -1;
		info.fmt_idx++;
	}
	va_end(ptr);
	return ((int)sum_print_bytes);
}
