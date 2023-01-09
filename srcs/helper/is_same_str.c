/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_same_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 12:05:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 16:04:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_same_str(const char *str1, const char *str2)
{
	const size_t	len1 = ft_strlen_ns(str1);
	const size_t	len2 = ft_strlen_ns(str2);

	return (len1 == len2 && !ft_strncmp_ns(str1, str2, len1));
}
