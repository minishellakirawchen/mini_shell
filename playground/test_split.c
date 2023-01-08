/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 15:24:47 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 17:59:10 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

size_t	count_sign(const char *src, char sign)
{
	size_t	cnt;
	size_t	idx;

	cnt = 0;
	idx = 0;
	if (!src)
		return (0);
	while (src[idx])
	{
		if (src[idx] == sign)
			cnt++;
		idx++;
	}
	return (cnt);
}

size_t	count_split_point_count_by_chr(const char *str, char chr, bool *is_error)
{
	size_t	j;
	size_t	cnt;

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
		if (j > 0 && str[j - 1] != chr)
		{
			cnt++;
//			printf("  # before cnt:%zu, str:%s: chr:%c\n", cnt, str, str[j]);
		}
		// syntax check
		if (str[j + 1] == chr) // <<
			j++;
		if (str[j + 1] == chr) // <<<
		{
			printf("syntax error\n");//TODO:error handling
			*is_error = true;
			return (0);
		}

		//後ろで区切る
		// <B, <<B, A<B, A<<B, A<, A<<
		// ^    ^    ^     ^    ^    ^ j
		if (str[j + 1] && str[j + 1] != chr)
		{
			cnt++;
//			printf("  # after  cnt:%zu, str:%s: chr:%c\n", cnt, str, str[j]);
		}
		j++;
	}
	return (cnt);
}



ssize_t	get_split_redirect_char_size(const char **cmds)
{
	size_t	cnt;
	size_t	i;
	bool	is_error;

	cnt = 0;
	is_error = false;
	if (!cmds)
		return (0);
	i = 0;
	while (cmds[i] && !is_error)
	{
		cnt++;
		if (count_sign(cmds[i], '<') != 0)
			cnt += count_split_point_count_by_chr(cmds[i], '<', &is_error);
		if (count_sign(cmds[i], '>') != 0)
			cnt += count_split_point_count_by_chr(cmds[i], '>', &is_error);
//		printf("cnt:%zu, cmd[i]:%s\n", cnt, cmds[i]);
		i++;
	}
	if (is_error)
		return (-1);
	return (cnt);
}

int	test_split_count(const char **src, ssize_t expectet, int test_no)
{
//	printf("\n");
	printf("[TEST %02d]\n", test_no);
	ssize_t ans = get_split_redirect_char_size(src);
	printf("  RES:%s, expected:%zd, ans:%zd\n\n", ans == expectet ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m", expectet, ans);
	if (ans == expectet)
		return (0);
	return (1);
}

int main(void)
{
	int	test_no = 1;
	int	ng = 0;
	const char *src1[10] = {"cat", "-e", "<", "test", ">", "out", NULL};
	const char *src2[10] = {"cat", "-e", "<test", ">out", NULL};
	const char *src3[10] = {"cat", "-e<", "test>", "out", NULL};
	const char *src4[10] = {"cat", "-e", "<<", "test", ">", "out", NULL};
	const char *src5[10] = {"cat", "-e", "<", "test", ">>", "out>>out", NULL};
	const char *src6[10] = {"cat", "-e", "<<", "test<<", "test>>", "out>out", NULL};
	const char *src7[10] = {"<in", ">test<", "in", "<<out>test>test>out", NULL};

	const char *src21[10] = {"cat", "-e", "<<<", "test", ">", "out", NULL};
	//{"<", "NUL"}, {"<", "<", "file"}などは次のパートでNG

	ng += test_split_count(src1, 6, test_no++);
	ng += test_split_count(src2, 6, test_no++);
	ng += test_split_count(src3, 6, test_no++);
	ng += test_split_count(src4, 6, test_no++);
	ng += test_split_count(src5, 8, test_no++);
	ng += test_split_count(src6, 10, test_no++);
	ng += test_split_count(src7, 14, test_no++);

	ng += test_split_count(src21, -1, test_no++);

	printf("## %s ##\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");
	return (0);
}



















