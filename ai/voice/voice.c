#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<pthread.h>
#include <signal.h>


int serial_fd;
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);


//	nset1 = set_opt(serial_fd, 115200, 8, 'N', 1);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if(tcgetattr( fd,&oldtio)  !=  0) 
    {
        perror("SetupSerial 1");
        return -1;
    }
    bzero(&newtio, sizeof( newtio ));
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(nBits)
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch(nEvent)
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E': 
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':  
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch(nSpeed)
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    if(nStop == 1)
        newtio.c_cflag &=  ~CSTOPB;
    else if (nStop == 2)
        newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    return 0;
}

void uart_init(void)
{
    int nset1,nset2;

    serial_fd = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY | O_NDELAY);
    if(serial_fd == -1)
    {
        printf("open() error\n");
        exit(1);
    }
    nset1 = set_opt(serial_fd, 115200, 8, 'N', 1);
    if(nset2 == -1)
    {
        printf("set_opt() error\n");
        exit(1);
    }
}


void volume(void)
{
    int len;
    unsigned char recv[3] = {'\0'};
    unsigned char msg0[3];
    unsigned char msg3[3];
    unsigned char msg1[3] = {0xA1, 0x01, 0x0b};
    unsigned char msg2[3] = {0xA2, 0x20, 0x21};



    /* 撒家是初始化指令  */
    msg0[0]=0xA0;
    msg0[1]=0xA0;	
    msg0[2]=0xA0;
    len = write(serial_fd,msg0,3);
    read(serial_fd, recv, 3);
    printf("1.init\t:%#X %#x %#x\n", recv[0], recv[1], recv[2]);
    printf("len=%d serial_fd=%d \n",len,serial_fd);
    usleep(100);

    /* 俺设置噪声大小 */
    len = write(serial_fd,msg1,3);
    read(serial_fd, recv, 3);
    printf("2.voice\t:%#X %#x %#x\n", recv[0], recv[1], recv[2]);
    printf("len=%d serial_fd=%d \n",len,serial_fd);
    usleep(100);

    /* 论家是在设置声音范围呀  */
    len = write(serial_fd,msg2,3);
    read(serial_fd, recv, 3);
    printf("3.voice\t:%#X %#x %#x\n", recv[0], recv[1], recv[2]);
    printf("len=%d serial_fd=%d \n",len,serial_fd);
    usleep(100);

    /* 连续识别语音指令 ***** */
    msg3[0]=0xAB;
    msg3[1]=0xAB;	
    msg3[2]=0x00;
    len = write(serial_fd,msg3,3);	
    read(serial_fd, recv, 3);
    printf("4.listen\t:%#X %#x %#x\n", recv[0], recv[1], recv[2]);
    printf("len=%d serial_fd=%d \n",len,serial_fd);
    usleep(100);
}

/* 函数介绍: 语音识别函数             ******** *
* 函数说明: 函数会识别语音指令并返回指令  *** */
int voice(void)
{
    unsigned char c;

    int len;
    while(1)
    {
        read(serial_fd,&c,1);
        usleep(500);
        if (0x5b==c)
        {
            read(serial_fd,&c,1);
            usleep(500);
            printf("识别指令值:%#02x\n", c+1);
            return c+1;
            break;
        }
    }	

    return 0;
}


