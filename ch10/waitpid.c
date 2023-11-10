#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char * argv[]) {
    int status;
    pid_t pid = fork(); //创建进程

    if (pid==0){ //子进程就等15秒再返回，为了验证waitpid不阻塞。
        sleep(15);
        return 24;
    }
    else {
        //函数中传递了-1，可以等待任何子进程终止
        //WNOHANG参数防止进入阻塞状态，此时没有子进程终止，就会返回0，while循环启动！
        while (!waitpid(-1, &status, WNOHANG)){
            sleep(3);
            puts("sleep 3 sec.");
        }
        //执行此代码时，说明子进程终止了。
        if (WIFEXITED(status))
            printf("Child send %d \n", WEXITSTATUS(status));
    }
    return 0;
}