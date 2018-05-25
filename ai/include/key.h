#ifndef _KEY_H
#define _KEY_H

#define  KEY_DEV_NODE "/dev/s5pc100_key"

void key_init(void);
void key_exit(void);

unsigned int key_read(void);

#endif
