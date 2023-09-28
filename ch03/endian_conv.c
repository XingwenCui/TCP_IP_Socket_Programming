#include<stdio.h>
#include<arpa/inet.h>

int main(int argc, char * argv[]) {
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port); //将主机port转换为net port字节序
    net_addr = htonl(host_addr); //同上，转换地址，和long类型
    
    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#lx \n", host_addr);
    printf("Network ordered address: %#lx \n", net_addr);

    return 0;
}