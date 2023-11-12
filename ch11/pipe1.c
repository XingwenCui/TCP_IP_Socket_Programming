#include<stdio.h>
#include<unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int fds[2];     // 保存文件描述符，在stack上声明（函数内部），无定义的初始值
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;

    // array的第一个是出口，第二个是入口
    pipe(fds);
    // fork后子进程用用刚才的两个fds描述符
    pid = fork();
    if (pid==0) {   // 子进程向管道传递string
        write(fds[1], str, sizeof(str));
    }
    else {          // 父进程从pipe中接收string
        read(fds[0], buf, BUF_SIZE);
        puts(buf);
    }
    return 0;
}