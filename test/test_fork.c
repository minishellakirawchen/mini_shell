#include "test.h"

int	main(void)
{
	int param = 0;

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
			printf("   child2 :%d\n", param);
			exit(0);
		}
		wait(NULL);
		exit(0);
	}
	wait(NULL);
	printf("parent :%d\n", param);

	return (0);
}