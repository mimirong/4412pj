#ifndef _LED_H_
#define _LED_H_


#define LED_NUM 4
#define LED_DEV_NODE "/dev/led"

#define CMD_LED_OFF _IO('k', 1)
#define CMD_LED_ON  _IO('k', 0)

void led_init(void);
void led_exit(void);
void led_on(unsigned int num);
void led_off(unsigned int num);

#endif
