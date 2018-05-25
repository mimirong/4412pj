#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "led.h"


static int fd_led;

/*
 * led初始化 打开led设备
 */
void led_init(void)
{
	fd_led = open(LED_DEV_NODE, O_RDWR);
	if (fd_led < 0) {
		perror("open led failed");
	}

}

/*
 * led设备关闭
 */
void led_exit(void)
{
	if (close(fd_led) < 0) {
		perror("close led failed");
	}
}


/**
 * 点亮对应编号的灯
 */
void led_on(unsigned int num)
{
	if (num > LED_NUM) {
		printf("please set the correct led number! \n");
		return;
	}
	ioctl(fd_led, CMD_LED_ON, (unsigned long)num);	
}


/**
 * 灭对应编号的灯
 */
void led_off(unsigned int num)
{
	if (num > LED_NUM) {
		printf("please set the correct led number! \n");
		return;
	}
	ioctl(fd_led, CMD_LED_OFF, (unsigned long)num);
}
