# include "test.h"

int main(void)
{
	char	*current_path;
	int		ret;

	printf("### cd test ###\n");
	current_path = getcwd(NULL, 0);
	printf(" current:%s\n", current_path);


	printf("cd\n");
	ret = chdir("/bin");//absolute path
	if (ret < 0)
		perror("chdir");

	current_path = getcwd(NULL, 0);
	printf(" moved:%s\n", current_path);

	char *cmd[5] = {"ls", NULL};
	printf("ls\n");
	execvp("ls", cmd);

	return (0);
}