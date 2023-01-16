/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 21:11:40 by takira            #+#    #+#             */
/*   Updated: 2023/01/13 22:05:26 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static bool	is_less_than_ll(long long before_x10, long long add, long long sign)
{
	long long	ov_div;
	long long	ov_mod;

	if (sign == 1)
	{
		ov_div = LONG_LONG_MAX / 10;
		ov_mod = LONG_LONG_MAX % 10;
	}
	else
	{
		ov_div = ((unsigned long)-LONG_LONG_MIN) / 10;
		ov_mod = ((unsigned long)-LONG_LONG_MIN) % 10;
	}
	if (before_x10 > ov_div)
		return (false);
	if (before_x10 == ov_div && add > ov_mod)
		return (false);
	return (true);
}

static int get_sign_and_increment_ptr(char *num, size_t *idx)
{
	long long	sign;

	sign = 1;
	if (num[*idx] == '-' || num[*idx] == '+')
	{
		if (num[*idx] == '-')
			sign = -1;
		*idx += 1;
	}
	return (sign);
}

long long	ft_strtoll(char *num, bool *is_success)
{
	long long	llnum;
	long long	sign;
	size_t		idx;

	*is_success = false;
	llnum = 0;
	idx = 0;
	while (isspace(num[idx]))
		idx++;
	sign = get_sign_and_increment_ptr(num, &idx);
	while (num[idx] && isdigit(num[idx]))
	{
		if (is_less_than_ll(llnum, num[idx] - '0', sign))
		{
			llnum = llnum * 10 + (long long)(num[idx] - '0');
			idx++;
			continue;
		}
		if (sign > 0)
			return (LONG_LONG_MAX);
		return (LONG_LONG_MIN);
	}
	if (!num[idx])
		*is_success = true;
	return (sign * llnum);
}

void test(char *num, int testno)
{
	bool	is_success;
	long long	ans = ft_strtoll(num, &is_success);
	printf("[test:%2d]\n", testno);
	printf(" input :%s\n", num);
	printf(" return:%lld, success:%d\n\n", ans, is_success);
}

int main(void)
{
	int testno = 1;

	char num1[30] = "0\0";
	char num2[30] = "1\0";
	char num3[30] = "-1\0";
	char num4[30] = "2147483647\0";
	char num5[30] = "-2147483645\0";
	char num6[30] = "\0";
	char num7[30] = "aa\0";
	char num8[30] = "9223372036854775807\0";
	char num9[30] = "-9223372036854775808\0";
	char num10[30] = "18446744073709551615\0";
	char num11[30] = "9223372036854775807hoge\0";

	test(num1, testno++);
	test(num2, testno++);
	test(num3, testno++);
	test(num4, testno++);
	test(num5, testno++);
	test(num6, testno++);
	test(num7, testno++);
	test(num8, testno++);
	test(num9, testno++);
	test(num10, testno++);
	test(num11, testno++);

}