#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "pwm.h"


static int fd_pwm;

/*
 * pwm初始化 打开pwm设备
 */
void pwm_init(void)
{
	fd_pwm = open(PWM_DEV_NODE, O_RDWR);
	if (fd_pwm < 0) {
		perror("open pwm failed");
	}

}

/*
 * pwm设备关闭
 */
void pwm_exit(void)
{
	if (close(fd_pwm) < 0) {
		perror("close pwm failed");
	}
}


/**
 * 点亮对应编号的灯
 */
void beep_on(void)
{

	ioctl(fd_pwm, BEEP_ON);	
}


/**
 * 灭对应编号的灯
 */
void beep_off(void)
{
	ioctl(fd_pwm, BEEP_OFF);
}

/**
 *设置pwm 频率 value :频率值
 */

void pwm_setfre(unsigned int value)
{
	ioctl(fd_pwm, SET_FRE, value);
}
