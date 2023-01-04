# include "test.h"

int main(void)
{
	char	*current_path;
	int		ret;

	printf("### cd test ###\n");
	current_path = getcwd(NULL, 0);
	printf(" current:%s\n", current_path);


	printf("cd\n");
	ret = chdir("/Users/akira/Documents/Programming/CLionProjects/42/42cursus/03_minishell/minishell/../");//absolute path
	if (ret < 0)
		perror("chdir");

	current_path = getcwd(NULL, 0);
	printf(" moved:%s\n", current_path);

	char *cmd[5] = {"ls", NULL};
	printf("ls\n");
	execve("/bin/ls", cmd, NULL);
	return (0);
}