/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:59:32 by takira            #+#    #+#             */
/*   Updated: 2022/10/28 17:59:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	maxsize(size_t a, size_t b)
{
	if (a >= b)
		return (a);
	return (b);
}

size_t	minsize(size_t a, size_t b)
{
	if (a <= b)
		return (a);
	return (b);
}
