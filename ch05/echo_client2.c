#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;//添加两个变量

    if (argc!=3){
        printf("Usage: %s <IP><port>\n", argv[0]);
        exit(1);
    }
    //创建面向连接的TCP socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    //向server发送连接请求
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    else   
        puts("Connected .....");
    //循环调用read，每次取1 Btye
    while (1){
        fputs("Input message(Q to quit): ",stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        //修改的代码
        //str_len是发送的代码，这是一次性传送的
        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        //这样不会发生异常的无限循环，如果是recv != str_len
        //那么当recv > str_len时，可能会一直循环出不去
        while (recv_len < str_len) {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE-1);
            if (recv_cnt == -1)
                error_handling("read() error!");
            recv_len += recv_cnt; //一直累加，当接受的不小于发送的，就停止read
        }
        message[str_len] = 0;
        printf("Message from server: %s \n", message);
    }
    // printf("Function read call count: %d \n", str_len);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}