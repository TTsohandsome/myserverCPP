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

struct sockaddr_in addr;
const int sport = 9527;

void sys_error(char *s) {
    perror(s);
    exit(1);
}

int main() {
    char buf[BUFSIZ];

    //socket地址初始化 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(sport);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    //创建socket
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
        sys_error("socket error");
    }

    //连接socket
    int sign = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
    if (sign == -1) {
        sys_error("connect error");
    }

    int cnt = 8; //通信次数
    while (cnt--) {
        write(cfd, "ciallott\n", 10);
        sign = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, sign);
        sleep(1);
    }
    close(cfd);
}

//socket()->connect()->write()->read()->close()