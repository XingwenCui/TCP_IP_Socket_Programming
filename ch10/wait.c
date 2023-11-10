#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]){
    int status;
    pid_t pid = fork();                 //创建进程

    if (pid==0) {                       //如果是子进程，子进程终止并返回3
        return 3;
    } else {                            //如果是父进程，就再创建一个子进程
        printf("Child PID: %d \n", pid);
        pid=fork();
        if (pid==0){                    //创建的子进程会在此终止并返回7
            exit(7); 
        } else {
            printf("Child PID: %d \n", pid);
            wait(&status);              //之前终止的子进程信息会保存到status，子进程也完全被销毁
            if (WIFEXITED(status))      //通过宏判断子进程是否正常退出，正常退出会返回true
                printf("Child send one: %d \n", WEXITSTATUS(status)); //并在此处返回刚才的返回值
            wait(&status);              //另一个终止的子进程也调用一下
            if (WIFEXITED(status))
                printf("Child send two: %d \n", WEXITSTATUS(status));
            sleep(30); 
        }
    }
    return 0;
}