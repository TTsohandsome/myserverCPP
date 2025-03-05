#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> //read/write
#include<errno.h>
#include<pthread.h>

const sport = 9527;
struct sockaddr_in saddr, caddr;

void sys_error(char *s) {
    perror(s);
    exit(1);
}

int main() {
    char buf[BUFSIZ];

    //socket地址初始化 
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(sport);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)  sys_error("socket error");
    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    listen(lfd, 128); //监听上限

    //阻塞等待
    socklen_t caddr_len = sizeof(caddr);
    int cfd = accept(lfd, (struct sockaddr*)&saddr, &caddr_len); //阻塞直至有客户端连接
    if (cfd == -1)  sys_error("accept error");

    while (1) {
        int len = read(cfd, buf, sizeof buf);
        write(STDOUT_FILENO, buf, len);

        for (int i = 0; i < len; i++) {
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, len); //返回数据
    } 
    
    close(lfd);
    close(cfd);
}