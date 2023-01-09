/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_split_size.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 09:34:46 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 10:38:39 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// pipe前後では区切る
static size_t	get_split_set_size(const char *str,  const char *set)
{
	size_t	cnt;
	size_t	idx;
	char	*setchr;

	cnt = 0;
	if (!str)
		return (0);
	idx = 0;
	printf(" #debug\n");
	printf("  #str:%s, len:%zu\n", str, strlen(str));
	while (str[idx])
	{
		// delim(space)抜けるまでインクリメント
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^                                    ^
		while (str[idx] && isspace(str[idx]))
			idx++;

		// set in str[idx]
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^                                    ^ set="
		if (str[idx] && strchr(set, str[idx]))
		{
			cnt++;
			printf("   # 1.set : cnt:%2zu, str[%zu]:%c\n", cnt, idx, str[idx]);
			setchr = strchr(set, str[idx]);
			idx++;
			// setを抜けるまでインクリメント
			// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
			//    ^                                            ^
			while (str[idx] && str[idx] != setchr[0])
				idx++;
			if (str[idx])
				idx++;
		}
		// space or setならばcontinue
		if (str[idx] && (isspace(str[idx]) || strchr(set, str[idx])))
			continue ;

		// space or setを抜けた後, 文字列があればcnt++
		// "foo"bar => A["foo"bar] or B["foo", bar] どっち？-> A
		// 		bash-3.2$ echo "hello" world //hello world
		// 		bash-3.2$ echo "hello"world // helloworld
		// この処理でset文字("")を残す場合、あとで削除する必要がある
		// set文字を消す場合、pipeの文字列 or 演算子の判定ができなくなる
		// -> tokenizeする方が確実か？
		// もしくは, expantionで結合するか？
		// とりあえず、""前後で区切り、""は残すこととする TODO:expantion

		// space or setを抜けた後, !str[idx]でなければcnt++
		if (str[idx])
		{
			cnt++;
			printf("   # 2.!set: cnt:%2zu, str[%zu]:%c\n", cnt, idx, str[idx]);
		}

		// pipeならpipeを抜けるまでインクリメント
		// [  echo hello | echo "foo | bar |"], [  " text "||| foo]
		//    ^
		if (str[idx] && str[idx] == '|')
		{
			while (str[idx] && str[idx] == '|')
				idx++;
			continue ; //space, setの確認から実行
		}
		// pipeでない場合
		// 次のpipe or space or setまでインクリメント
		if (str[idx] && str[idx] != '|')
		{
			while (str[idx] && str[idx] != '|' && !isspace(str[idx]) && !strchr(set, str[idx]))
				idx++;
			continue ;
		}
	}
	return (cnt);
}


int	test_split_count(const char *src, ssize_t expectet, int test_no)
{
//	printf("\n");
	printf("[TEST %02d]\n", test_no);
	ssize_t ans = get_split_set_size(src, "\"\'");
	printf("  RES:%s, expected:%zd, ans:%zd\n\n", ans == expectet ? "\x1b[32mAC\x1b[0m" : "\x1b[31mWA\x1b[0m", expectet, ans);
	if (ans == expectet)
		return (0);
	return (1);
}

int main(void)
{
	int	test_no = 1;
	int	ng = 0;
	const char *src1 = "echo hello world";
	const char *src2 = "echo hello world |";
	const char *src3 = "echo hello world |||";
	const char *src4 = "echo hello world | grep a";
	const char *src5 = "echo hello world | grep a | echo \"foo bar\"";
	const char *src6 = "echo hello world | grep a | echo \"foo | bar\"";
	const char *src7 = "|echo hello world | grep a | echo \"foo | bar\"|";
	const char *src8 = "\"test'foo'|||| || \"";


	//{"<", "NUL"}, {"<", "<", "file"}などは次のパートでNG

	ng += test_split_count(src1, 3, test_no++);
	ng += test_split_count(src2, 4, test_no++);
	ng += test_split_count(src3, 4, test_no++);
	ng += test_split_count(src4, 6, test_no++);
	ng += test_split_count(src5, 9, test_no++);
	ng += test_split_count(src6, 9, test_no++);
	ng += test_split_count(src7, 11, test_no++);
	ng += test_split_count(src8, 1, test_no++);

	printf("## %s ##\n", ng == 0 ? "\x1b[32mALL AC !!!\x1b[0m" : "\x1b[31mWA :X\x1b[0m");
	return (0);
}



