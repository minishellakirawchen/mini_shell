#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

volatile sig_atomic_t g_flag = 0;

void signal_handler(int signum) {
    g_flag = 1;
    printf("SIGNAL!!!\n");
    printf("SIGNUM: %d\n", signum);
    exit(0);
}


int main()
{
    char str[100];
    int status;

    signal(SIGINT, SIG_IGN);
    while (fgets(str, 100, stdin) || !g_flag)
    {
        printf("%s\n", str);
        pid_t pid = fork();
        if (pid == 0)
        {
            signal(SIGINT, signal_handler);
            while (1)
            {
                printf("child\n");
            }
        }
        pid_t r = waitpid(pid, &status, 0);
        if (r < 0) {
            perror("waitpid");
            exit(-1);
        }
        if (WIFEXITED(status)) {
            // 子プロセスが正常終了の場合
            int exit_code = WEXITSTATUS(status); //子プロセスの終了コード
            printf("child exit-code=%d\n", exit_code);
        } else {
            printf("child status=%04x\n", status);
        }
        printf("parent process end\n");
            return 0;
        }

}
