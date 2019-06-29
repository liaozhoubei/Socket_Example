#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(int argc, const char* argv[])
{
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket error");
        exit(1);
    }

    unlink("client.sock");

    // ================================
    // 给客户端绑定一个套接字文件
    struct sockaddr_un client;
    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, "client.sock");
    int ret = bind(fd, (struct sockaddr*)&client, sizeof(client));
    if(ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // 初始化server信息
    struct sockaddr_un serv;
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "server.sock");

    // 连接服务器
    connect(fd, (struct sockaddr*)&serv, sizeof(serv));

    // 通信
    while(1)
    {
        char buf[1024] = {0};
        fgets(buf, sizeof(buf), stdin);
        send(fd, buf, strlen(buf)+1, 0);

        // 接收数据
        recv(fd, buf, sizeof(buf), 0);
        printf("recv buf: %s\n", buf);
    }

    close(fd);

    return 0;
}
    
