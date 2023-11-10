#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
//与之前不同的部分
#include<signal.h>  // 信号库
#include<sys/wait.h> // 调用waitpid函数
#define BUF_SIZE 30

/*
	实现简单并发echo服务器代码，主要逻辑由以下3点：
	- server父进程调用accept函数受理连接请求
	- 此时获取的socket文件描述符创建并传递给子进程
	- 子进程利用传递来的文件描述符提供服务
*/
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[]){
    // 创建服务端客户端socket描述符
	int serv_sock, clnt_sock; 
    struct sockaddr_in serv_adr, clnt_adr; // 地址结构体
	socklen_t adr_sz;
    char buf[BUF_SIZE];		// 缓冲区
    int str_len;
    int state;
	// 设置sigaction结构体
    struct sigaction act;
    pid_t pid;				// 进程ID

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	// 为防止僵尸进程
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);

	//调用socket 函数创建服务器
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	// 绑定socket
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	//调用bind分配ip，port
	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	//调用listen，此时才是服务器端的socket
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// 主循环，不断等待新的客户端链接，对于每个链接，创建一个新的子进程来处理客户端请求
	while (1){
		adr_sz = sizeof(clnt_adr);
		//调用accept，取一个链接请求与client链接，并返回创建的套接字
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			puts("new client connected..");
		// 调用fork可以让父子进程分别带有创建socket时生成的文件描述符
		pid=fork();
		if(pid==-1){
			close(clnt_sock);
			continue;		// 保持serv运行
		}
    	if (pid==0) {	// 子进程运行区域
			close(serv_sock);
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
				write(clnt_sock, buf, str_len);
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else 
			close(clnt_sock);
	}
	
	close(serv_sock);
	return 0;
}

// 错误处理函数
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// 信号处理函数，捕捉SIGCHLD信号
void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid=waitpid(-1, &status, WNOHANG);
    printf("remove proc id: %d \n", pid);
}