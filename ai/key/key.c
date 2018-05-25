#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "key.h"

static int fd_key;

/*
 * 按键初始化 打开按键设备
 */
void key_init(void)
{
	fd_key = open(KEY_DEV_NODE, O_RDWR);
	if (fd_key < 0) {
		perror("open key failed");
	}

}

/*
 * 按键设备关闭
 */
void key_exit(void)
{
	if (close(fd_key) < 0) {
		perror("close key failed");
	}
}

/**
 * 读键值，在驱动中如果没有新按键按下，会阻塞
 * return : key_value  -1: 读取失败 >0:键值
 */
unsigned int key_read(void)
{
	int ret;
	unsigned int key_value;
	key_value = -1;
	
	ret = read(fd_key, &key_value, sizeof(key_value));
	if (ret < 0) {
		perror("key read failed");
	}
	return key_value;
}


