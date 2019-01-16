socket ( 建立一个 socket 通信 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数：
		int socket(int domain, int type, int protocol);
	参数分析：
		domain --> 定使用何种的地址类型, 完整的定义在/usr/include/bits/socket.h 内, 底下是常见的协议:
			PF_UNIX/PF_LOCAL/AF_UNIX/AF_LOCAL UNIX 进程通信协议
			PF_INET/AF_INET Ipv4 网络协议
			PF_INET6/AF_INET6 Ipv6 网络协议
			PF_IPX/AF_IPX IPX-Novell 协议
			PF_NETLINK/AF_NETLINK 核心用户接口装置
			PF_X25/AF_X25 ITU-T X. 25/ISO-8208 协议
			PF_AX25/AF_AX25 业余无线 AX. 25 协议
			PF_ATMPVC/AF_ATMPVC 存取原始 ATM PVCs
			PF_APPLETALK/AF_APPLETALK appletalk (DDP)协议
			PF_PACKET/AF_PACKET 初级封包接口
		type --> 有下列几种数值:
			SOCK_STREAM 提供双向连续且可信赖的数据流, 即 TCP. 支持 OOB 机制, 在所有数据传送前必须使用 connect()来建立连线状态.
			SOCK_DGRAM 使用不连续不可信赖的数据包连接
			SOCK_SEQPACKET 提供连续可信赖的数据包连接
			SOCK_RAW 提供原始网络协议存取
			SOCK_RDM 提供可信赖的数据包连接
			SOCK_PACKET 提供和网络驱动程序直接通信
		protocol -->  用来指定 socket 所使用的传输协议编号, 通常此参考不用管它, 设为 0 即可.
	返回值：
		成功则返回 socket 处理代码(待链接的套接字)
		失败返回-1.
			错误代码 EPROTONOSUPPORT 参数 domain 指定的类型不支持参数 type 或 protocol 指定的协议
			ENFILE 核心内存不足, 无法建立新的 socket 结构
			EMFILE 进程文件表溢出, 无法再建立新的 socket
			EACCESS 权限不足, 无法建立 type 或 protocol 指定的协议
			ENOBUFS/ENOMEM 内存不足
			EINVAL 参数 domain/type/protocol 不合法

----------------------------------------------------------------

connect ( 建立 socket 连线 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int connect(int sockfd, struct sockaddr * serv_addr, int addrlen);
	参数分析：
		sockfd --> 待连接套接字
		serv_addr --> 包含对端地址（IP+PORT）的通用地址结构体的指针(sockaddr_in结构体)
		addrlen --> 地址结构体大小
	返回值：
		成功：0
		失败：-1

----------------------------------------------------------------

atoi ( 将字符串转换成整型数 )
	头文件:
		#include <stdlib.h>
	定义函数:
		int atoi(const char *nptr);
	参数分析：
		nptr --> 需要转换的字符串，跳过前面的空格字符, 直到遇上数字或正负符号才开始做转换, 而再遇到非数字或字符串结束时('\0')才结束转换
	返回值：
		返回转换后的整型数

----------------------------------------------------------------

htons ( 将 16 位主机字符顺序转换成网络字符顺序 )
	头文件:
		#include <netinet/in.h>
	定义函数:
		unsigned short int htons(unsigned short int hostshort);
	参数分析：
		hostshort --> 需要转换的16位主机序整数
	返回值：
		返回对应的网络字符顺序

----------------------------------------------------------------

htonl ( 将 32 位主机字符顺序转换成网络字符顺序 ）
	头文件：
		#include <netinet/in.h>
	定义函数：
		unsigned long int htonl(unsigned long int hostlong);
	参数分析：
		hostlong --> 需要转换的32位主机序整数
	返回值：
		返回对应的网络字符顺序

----------------------------------------------------------------

inet_addr ( 将网络地址转成二进制的数字 )
	头文件：
		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <arpa/inet.h>
	定义函数：
		unsigned long int inet_addr(const char *cp);
	参数分析：
		cp --> 网络地址字符串转换成网络所使用的二进制数字(需要转换的点分十进制网路地址)
	返回值：
		成功则返回对应的网络二进制的数字
		失败返回-1.

----------------------------------------------------------------

inet_ntoa ( 将网络二进制的数字转换成网络地址 ）
	头文件：
		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <arpa/inet.h>
	定义函数：
		char * inet_ntoa(struct in_addr in);
	参数分析：
		in --> 需要转换的二进制网路地址
	返回值：
		成功则返回字符串指针
		失败则返回 NULL.

----------------------------------------------------------------

bind ( 对 socket 定位 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int bind(int sockfd, struct sockaddr * my_addr, int addrlen);
	参数分析：
		sockfd --> socket 处理代码 (socket的返回值待链接的套接字)
		my_addr --> 包含本地地址（IP+PORT）的通用地址结构体的指针(sockaddr_in结构体)
		addrlen --> 结构体长度
	返回值：
		成功：0
		失败：-1

----------------------------------------------------------------

listen ( 等待连接 )
	头文件:
		#include <sys/socket.h>
	定义函数:
		int listen(int s, int backlog);
	参数分析：
		s --> 待连接套接字
		backlog --> 最大同时接收连接请求个数
	返回值：
		成功返回0，并将 sockfd 设置为监听套接字
		失败返回-1
备注：
由于历史原因，各种系统对 backlog 的理解并不一致，以 LINUX 为例，监听端能同时接收的
最大连接请求个数为 backlog+4

----------------------------------------------------------------

accept ( 接受 socket 连线 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int accept(int s, struct sockaddr * addr, int * addrlen);
	参数分析：
		s --> 待连接套接字
		addr --> 包含对端地址（IP+PORT）的通用地址结构体的指针(sockaddr_in结构体)
		addrlen --> 结构体长度
	返回值：
		已连接套接字（非负整数）
		失败返回-1

----------------------------------------------------------------

recv ( 经 socket 接收数据 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int recv(int s, void *buf, int len, unsigned int flags);
	参数分析：
		s --> 已连接套接字 
		buf --> 存储数据的缓冲区
		len --> 缓冲区大小
		flags --> 接收标志 一般设 0. 其他数值定义如下:
			MSG_OOB 接收以 out-of-band 送出的数据.
			MSG_PEEK 返回来的数据并不会在系统内删除, 如果再调用 recv()会返回相同的数据内容.
			MSG_WAITALL 强迫接收到 len 大小的数据后才能返回, 除非有错误或信号产生.
			MSG_NOSIGNAL 此操作不愿被 SIGPIPE 信号中断返回值成功则返回接收到的字符数, 失败返回-1,
	返回值：
		成功：已接收字节数
		失败：-1
		断开：返回 0

----------------------------------------------------------------

recvfrom ( 经 socket 接收数据 )
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int recvfrom(int s, void *buf, int len, unsigned int flags, struct sockaddr *from,int *fromlen);
	参数分析：
		s --> 接收的socket套接字
		buf --> 接收到的信息存放到哪里
		len --> 接收信息的长度
		flags --> flags 一般设 0. 其他数值定义如下:
				MSG_OOB 接收以 out-of-band 送出的数据.
				MSG_PEEK 返回来的数据并不会在系统内删除, 如果再调用 recv()会返回相同的数据内容.
				MSG_WAITALL 强迫接收到 len 大小的数据后才能返回, 除非有错误或信号产生.
				MSG_NOSIGNAL 此操作不愿被 SIGPIPE 信号中断返回值成功则返回接收到的字符数, 失败返回-1,
		from --> 信息来源哪里
		fromlen --> 地址结构体的长度
	返回值：
		成功则返回接收到的字符数
		失败则返回-1, 错误原因存于 errno 中.

----------------------------------------------------------------

sendto ( 经 socket 传送数据 ) 
	头文件:
		#include <sys/types.h>
		#include <sys/socket.h>
	定义函数:
		int sendto(int s, const void * msg, int len, unsigned int flags, const struct sockaddr * to, int tolen);
	参数分析：
		s --> 发送信息到那个socket
		msg  --> 待发送的信息
		len  --> 需要发送信息的长度
		flags  --> flags 一般设 0, 其他数值定义如下
				MSG_OOB 传送的数据以 out-of-band 送出.
				MSG_DONTROUTE 取消路由表查询
				MSG_DONTWAIT 设置为不可阻断运作
				MSG_NOSIGNAL 此动作不愿被 SIGPIPE 信号中断.
		to  --> 发送信息到哪个地址
		tolen  --> 地址长度
	返回值：	
		成功则返回实际传送出去的字符数
		失败返回－1, 错误原因存于 errno 中.
====================
结构体
====================
特殊地址结构体 —— IPv4 地址结构体：
struct sockaddr_in
{
	u_short sin_family; // 地址族
	u_short sin_port; // 端口
	struct in_addr ; // IPV4 地址
	char sin_zero[8];
};

struct in_addr
{
	in_addr_t s_addr; // 无符号 32 位网络地址
};

------------------------------------
linux平台下：
下面两个头文件都有定义：
1）#include <sys/socket.h>
2）#include <unistd.h>

详细定义：typedef int socklen_t;
------------------------------------