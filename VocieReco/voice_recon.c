//#include "voice_recon.h"
#include <stdio.h>

/**
 * 记录音频
 * 
 * @author gec (19/01/19)
 * 
 * @param duration 记录时长
 * 
 * @return int 0 成功 
 */
static int record_voice(int duration)
{
	static int voice_num = 0;
	char cmd[60];
	sprintf(cmd, "arecord - d%d - c1 - r16000 - twav - fS16_LE %d.wav", duration,
			voice_num++);
	system(cmd);
	return 0;
}

static int init()
{
	return 0;
}

static int desotry()
{
	return 0;
}



int main(void)
{

	return 0;
}
