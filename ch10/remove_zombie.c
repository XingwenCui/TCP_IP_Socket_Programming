#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
    {
        printf("Removed proc id: %d \n", id);             //子进程的 pid
        printf("Child send: %d \n", WEXITSTATUS(status)); //子进程的返回值
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) //子进程执行阶段
    {
        puts("Hi I'm child process1");
        sleep(10);
        return 12;
    }
    else //父进程执行阶段
    {
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if (pid == 0)
        {
            puts("Hi! I'm child process2");
            sleep(11); //同时设为10另一个可能不显示，
            exit(24);
        }
        else
        {
            
            printf("Child proc id: %d \n", pid);
            for (int i = 0; i < 5; i++)
            {
                printf("wait %d\n", i);
                sleep(5);
            }
        }
    }
    return 0;
}