/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:32:45 by takira            #+#    #+#             */
/*   Updated: 2023/01/17 22:40:11 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{

	//char *path;
	/* fork cd, touch playground 22/Jan/5th */
	int param = 0;
	//char *touch_cmd[10] = {"touch", "testfile", NULL};
	//char *ls_cmd[10] = {"ls", NULL};
/*
	char *cd_cmd[10] = {"cd", "./abc", NULL};
	int pid_t = fork();
	if (pid_t == 0)
	{
		// child
		printf("child cd\n");
		execvp(cd_cmd[0], cd_cmd);
		printf("child pwd\n");				// not print
		path = getcwd(NULL, 0);				// not print
		printf("path: %s\n", path);			// not print
		printf("child after execvp\n");		// not print
		// cd -> pwd 不明
		exit(0);
	}
	wait(NULL);
	printf("parent pwd\n");
	path = getcwd(NULL, 0);
	printf("path: %s\n", path);
*/


	// int pid_t = fork();
	// if (pid_t == 0)
	// {
	// 	// child
	// 	printf("child touch\n");//childでもfile反映される
	// 	execvp(touch_cmd[0], touch_cmd);
	// 	printf("child after exexcp\n"); //not print it
	// 	exit(0);
	// }
	// wait(NULL);
	// printf("parent ls\n");
	// execvp(ls_cmd[0], ls_cmd);
	// printf("parent after exexcp\n"); //not print it


	/* fork playground 22/Jan/4th */
	printf("init :%d\n", param);
	int pid_t = fork();
	if (pid_t == 0)//child 1
	{
		printf("  child1 :%d\n", param);
		param++;
		printf("  child1 :%d\n", param);

		int pid_t2 = fork();
		if (pid_t2 == 0)// child2
		{
			printf("    child2 :%d\n", param);
			param++;
			printf("    child2 :%d\n", param);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
	wait(NULL);
	printf("parent :%d\n", param);

	return (0);
}
