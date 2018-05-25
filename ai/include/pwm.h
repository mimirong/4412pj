#ifndef _PWM_H
#define _PWM_H

#define PWM_DEV_NODE "/dev/pwm"

#define magic_number 'k'
#define BEEP_ON _IO(magic_number,0)                                                                            
#define BEEP_OFF _IO(magic_number,1)
#define SET_FRE _IO(magic_number,2)

void pwm_init(void);
void pwm_exit(void);

void beep_on(void);
void beep_off(void);
void pwm_setfre(unsigned int value);
#endif
