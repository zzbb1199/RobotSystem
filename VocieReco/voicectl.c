#include "common.h"

#define REC_CMD  "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"

int main(int argc, char const *argv[]) // ./wav2pcm ubuntu-IP
{
	int id_num = 0, num;
	if(argc != 2)
	{
		printf("Usage: %s <ubuntu-IP>\n", argv[0]);
		exit(0);
	}

	int sockfd = init_sock(argv[1]); //由命令行传入一个对方的IP 等效于socket+bind+listen+accept

	printf("waiting for result...\n");
	while(1)
	{
		// 1，调用arecord来录一段音频
		printf("input num to start REC in 3s...\n");
		scanf("%d", &num);

		//启动录音
		system(REC_CMD);

		// 2，将录制好的PCM音频发送给语音识别引擎
		send_pcm(sockfd, PCM_FILE);

		// 3，等待对方回送识别结果（字符串ID）
		xmlChar *id = wait4id(sockfd);
		if(id == NULL) continue;

		//将字符串的id转化成为整形的id
		id_num = atoi((char *)id);
		if(id_num == 999)
		{
			printf("bye-bye!\n");
			system("killall -9 madplay");
		}

		if(id_num == 5)
		{
			system("madplay /Even/mp3/2.mp3 &");
		}

		printf("%d\n", id_num);

	}
	close(sockfd);
	return 0;
}


