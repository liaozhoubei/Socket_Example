#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 8000
typedef struct {
	char name[30];
	int age;
}People;
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
	// 设置一个固定的结构体
	People p;
	memset(&p,0,sizeof(p));
	strcpy(p.name, "Ken stavin");
	p.age=20;

    while (fgets(buf, sizeof(buf), stdin)) {
        
		// 发送固定结构体
		write(sfd, &p, sizeof(People));
		// 接受从服务端发送的数据
        len = read(sfd, buf, sizeof(buf));
		// 将服务端数据输出到屏幕中
        write(STDOUT_FILENO, buf, len);
		// 手动换行
		printf("\n ");
    }
    return 0;
}
