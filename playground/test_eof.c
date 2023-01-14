/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_eof.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 18:15:55 by takira            #+#    #+#             */
/*   Updated: 2023/01/14 18:25:06 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
# include <readline/readline.h>

int main(void)
{
	char *line;
	printf("EOF c:%c, d:%d, x:%x\n", EOF, EOF, EOF);

	while (true)
	{
		line = readline(">");
		printf("input:%s\n", line);
		if (!line)
		{
			printf("NULL break\n"); //<- EOFはNULL break
			break ;
		}
		if (line[0] == EOF)
		{
			printf("EOF break\n");
			break ;
		}
	}
	return (0);
}