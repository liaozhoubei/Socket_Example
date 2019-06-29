#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 8000

int main(int argc, char *argv[])
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[4096];

    if (argc < 2) {
        printf("./client serv_ip\n");
        return 1;
    }
	// AF_INET ipv4协议
	// SOCK_STREAM	tcp连接类型
    sfd = socket(AF_INET, SOCK_STREAM, 0);
	// 将结构体	serv_addr 清零
    bzero(&serv_addr, sizeof(serv_addr));
	// 设置结构体信息
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
	// 将主机字节序转为网络字节序
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
	// 连接服务端
    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (fgets(buf, sizeof(buf), stdin)) {
		// 读取从控制台输入的字符串，发送给服务端
        write(sfd, buf, strlen(buf));
        // 接受从服务端发送的数据
		len = read(sfd, buf, sizeof(buf));
		// 将服务端数据输出到屏幕中
        write(STDOUT_FILENO, buf, len);
    }
    return 0;
}
