#include "socket.h"

int main(int argc, char const *argv[])
{

    //打造自己的邮箱
    int fd_socket = socket( AF_INET , SOCK_DGRAM , 0);
    if (fd_socket < 0 )
    {
        printf("socket failed !\n");
        return -1 ;
    }


    // struct sockaddr_in
    // {
    // 	u_short sin_family; // 地址族
    // 	u_short sin_port; // 端口
    // 	struct  sin_addr; // IPV4 地址
    // 	char sin_zero[8];
    // };

    // struct in_addr
    // {
    // 	in_addr_t s_addr; // 无符号 32 位网络地址
    // };

    //绑定邮箱地址
    struct sockaddr_in my_addr ; //自己的地址结构体
    struct sockaddr_in Tie_addr ; //对方的地址结构体
    //配置地址信息
    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(65000) ;
    my_addr.sin_addr.s_addr = inet_addr("172.16.1.1");

    int addrlen = sizeof(struct sockaddr_in);
    //绑定
    int ret = bind(fd_socket, (struct sockaddr *)&my_addr, addrlen);
    if (-1 == ret )
    {
        printf(" Bind failed !!\n");
        return -1 ;
    }else{
        printf("Bind Succeed !!\n");
    }


    //坐等来信
    char msg [100];
    while(1)
    {
        printf("reviifldsjflsdjaflkjaslf\n");
        bzero( msg, 100 );
        ret = recvfrom(fd_socket, msg, 100 , 0, (struct sockaddr *)&Tie_addr, &addrlen);
        if (-1  == ret  )
        {
            perror("recv error \n");
            continue ;
        }

        printf("msg:%s\naddr:%s\n\nport:%d",
                msg,
                inet_ntoa(Tie_addr.sin_addr),
                ntohs(Tie_addr.sin_port));





    }


    return 0;
}
