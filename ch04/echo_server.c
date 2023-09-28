#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 10
void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;
    char message[BUF_SIZE];
    int str_len;

	struct sockaddr_in serv_addr; //服务端地址信息
	struct sockaddr_in clnt_addr; //客户端地址信息
	socklen_t clnt_addr_size;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	//调用socket 函数创建
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//调用bind分配ip，port
	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	//调用listen，此时才是服务器端的socket
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
    for (int i = 0; i < 5; i++) {
        //调用accept，取一个链接请求与client链接，并返回创建的套接字
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i+1);
        //完成echo服务，原封不动读取string
        while ((str_len = read(clnt_sock, message, BUF_SIZE))!=0)
            write(clnt_sock, message, str_len);
        close(clnt_sock);
    }
	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

