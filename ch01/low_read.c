#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
void error_handling(char *message);
#define BUF_SIZE 100

int main(void) {
    int fd;
    char buf[] = "Let's go!\n";

    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd);

    if (read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error!");
    
    printf("file data: %s", buf);
    close(fd);
    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}