    #include<stdio.h>
    #include<unistd.h>
    #include<signal.h>

    //定义信号处理函数，这种类型的函数叫信号处理器，
    void timeout(int sig){
        if (sig==SIGALRM)
            puts("Time out!");
        alarm(2);    //每隔2s发出一个SIGALRM信号
    }
    void keycontrol(int sig) {
        if (sig==SIGINT)
            puts("CTRL+C pressed");
    }

    int main(int argc, char *argv[]) {
        //注册这两个信号及其处理器
        signal(SIGALRM, timeout);
        signal(SIGINT, keycontrol);
        alarm(2); //2秒后发生SIGALRM信号，这样触发第一个注册的信号
                //此时虽然正在执行for循环的sleep，但触发信号后就唤醒了程序
                //之后再次进入循环，timeout函数会再次设置alarm，所以可以很快结束整个程序
        //为了查看信号处理器执行，提供3次等待时间。
        for (int i = 0; i < 3; i++) {
            puts("wait...");
            sleep(20);
        }
        return 0;
    }