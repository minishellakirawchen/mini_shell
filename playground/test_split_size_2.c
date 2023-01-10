/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split_size_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 11:18:22 by takira            #+#    #+#             */
/*   Updated: 2023/01/10 11:32:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

ssize_t	count_split_point_count_by_chr(const char *str, char chr)
{
	size_t	j;
	ssize_t	cnt;

//	*is_error = false;
	cnt = 0;
	j = 0;
	while (str[j])
	{
		while (str[j] && str[j] != chr)
			j++;
		if (!str[j])
			break ;

		//前で区切る
		// <A, <<A, A<B, A<<B, A<, A<<
		// ^   ^     ^    ^     ^   ^ j
		if (str[j] == chr && j > 0 && str[j - 1] != chr)
			cnt++;

		//後ろで区切る
		// <B, <<B, A<B, A<<B, A<, A<<
		// ^    ^    ^     ^    ^    ^ j
//		if (str[j] && str[j + 1] && str[j + 1] != chr)
//			cnt++;
		if (str[j] && str[j] == chr)
		{
			while (str[j] && str[j] == chr) // <<
				j++;
			if (str[j] && str[j] != chr)
			{
				cnt++;
				continue ;
			}
		}
		j++;
	}
	return (cnt);
}

int	test_split_count(const char *src, ssize_t expectet, int test_no)
{
//	printf("\n");
	printf("[TEST %02d]\n", test_no);
	ssize_t ans = count_split_point_count_by_chr(src, '<');
	printf("  RES:%s, expected:%zd, ans:%zd\n\n", ans == expectet ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m", expectet, ans);
	if (ans == expectet)
		return (0);
	return (1);
}

int main(void)
{
	int	test_no = 1;
	int	ng = 0;
	const char *src1 = "<<a";


	//{"<", "NUL"}, {"<", "<", "file"}などは次のパートでNG

	ng += test_split_count(src1, 2, test_no++);

	printf("## %s ##\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");
	return (0);
}



