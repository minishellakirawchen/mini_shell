/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_string.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:17:23 by takira            #+#    #+#             */
/*   Updated: 2022/10/27 16:17:28 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

ssize_t	print_c(char chr, t_printf_info info)
{
	ssize_t	ret_bytes;
	size_t	printsize;

	ret_bytes = 0;
	printsize = maxsize(info.width_siz, 1);
	if (printsize == 1)
		return (ft_putchar_for_printf(chr, 1));
	if (info.flag_left && errno == 0)
	{
		ret_bytes += ft_putchar_for_printf(chr, 1);
		printsize--;
	}
	while (printsize-- > !info.flag_left && errno == 0)
		ret_bytes += ft_putchar_for_printf(' ', 1);
	if (!info.flag_left && errno == 0)
		ret_bytes += ft_putchar_for_printf(chr, 1);
	return (ret_bytes);
}

ssize_t	print_s(char *str, t_printf_info info)
{
	ssize_t	ret_bytes;
	size_t	strlen;
	size_t	printsize;
	size_t	padlen;

	ret_bytes = 0;
	if (!str)
		str = "(null)";
	strlen = ft_strlen_printf(str);
	if (info.prec_dot)
		strlen = minsize(strlen, info.perc_siz);
	printsize = maxsize(strlen, info.width_siz);
	padlen = 0;
	if (printsize > strlen)
		padlen = printsize - strlen;
	if (info.flag_left)
		while (strlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(*str++, 1);
	if (padlen)
		while (padlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(' ', 1);
	if (!info.flag_left)
		while (strlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(*str++, 1);
	return (ret_bytes);
}

ssize_t	print_percent(t_printf_info info)
{
	ssize_t	ret_bytes;
	size_t	printsize;
	size_t	padlen;

	ret_bytes = 0;
	if ((info.flag_left || !info.width_siz) && info.flag_zero_pad)
		info.flag_zero_pad = 0;
	printsize = maxsize(info.width_siz, 1);
	if (printsize == 1 && errno == 0)
		return (ft_putchar_for_printf('%', 1));
	padlen = printsize - 1;
	if (info.flag_left && errno == 0)
		ret_bytes += ft_putchar_for_printf('%', 1);
	if (info.flag_zero_pad)
		while (padlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf('0', 1);
	else
		while (padlen-- && errno == 0)
			ret_bytes += ft_putchar_for_printf(' ', 1);
	if (!info.flag_left && errno == 0)
		ret_bytes += ft_putchar_for_printf('%', 1);
	return (ret_bytes);
}
