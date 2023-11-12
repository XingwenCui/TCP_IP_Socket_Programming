#include<stdio.h>
#include<unistd.h>
#define BUF_SIZE 30

int main (int argc, char *argv[]) {
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank u for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    // 创建管道
    pipe(fds);
    // 创建子进程，并复制管道描述符
    pid = fork();
    if (pid==0) {
        // 传输数据
        write(fds[1], str1, sizeof(str1));
        sleep(2);   // 这里sleep是为了让父进程先读取到数据
        // 接收数据，如果注释了上边代码，下边的read会先read pipe中的数据
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n",buf);
    }
    else {
        read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output: %s \n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);  // 父进程结束后shell会弹出命令提示符，之后子进程才会输出，为了避免就多sleep1秒
    }

    return 0;
}