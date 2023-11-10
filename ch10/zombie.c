#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork(); //创建进程

    if (pid==0)
        puts("Hi, I am a child process"); //向终端输出字符串，并换行
    else {
        printf("Child Pid: ID: %d \n", pid); //通过查看该pid判断是否为zombie
        sleep(30); //父进程暂停30秒，当父进程被终止，zombie将同时销毁
    }
    if (pid == 0)
        puts("END child process");
    else
        puts("END parent process");
    return 0;
}