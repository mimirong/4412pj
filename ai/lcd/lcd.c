#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/ioctl.h>

#include "lcd.h"
#include "lcd_font.h"
#include "bmp.h"

static struct fb_var_screeninfo 	vinfo;
struct fb_fix_screeninfo fbi_fix;//����̶�����

static unsigned char			*fb_buf;



static unsigned char *map_fb(void)
{
	int	fd,ret;
	int	screensize;
	unsigned char	*pfb;

	fd	= open("/dev/fb0", O_RDWR);
	if (!fd)
	{
		printf("Error: cannot open fb device - %s.\n", (char*)strerror(errno));
		exit(1);
	}
	printf("Open fb device success!\n");

	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))
	{
		printf("Error reading variable information - %s.\n", (char*)strerror(errno));
		exit(1);
	}

	if (vinfo.bits_per_pixel != 32)//32
	{
		printf("Error:not supported bpp, it only supports 16-bit color.\n");
		exit(1);
	}

	printf("Xsize:%d Ysize%d\n", vinfo.xres, vinfo.yres);

	ret = ioctl(fd,FBIOGET_FSCREENINFO,&fbi_fix);
	if(ret < 0)
	{		
		perror("ioctl");		
		exit(1);
	}
	printf("fbi.fix.smem_start:%lu ,fbi.fix.smem_len:%lu\n",fbi_fix.smem_start ,fbi_fix.smem_len);

	// figure out the size of screen of bytes
//	screensize = (int)(vinfo.xres * vinfo.yres * 4);	// 16bpp 2-byte per pixel
	screensize = fbi_fix.smem_len;
	pfb	= (unsigned char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((int)pfb == -1)
	{
		printf("Error: fail to map fb to memory - %s.\n", (char*)strerror(errno));
		exit(4);
	}
	printf("The fb device mapped to memory success. screen size = %d \n", screensize);

	return pfb;
}

//��ʼ������
int LCD_Init(void)
{
	fb_buf = map_fb();
	if (fb_buf == NULL)
	{
		printf("Map fb failed..\n");
		return -1;
	}
	return 0;

}

/****************************************************************************
* ��    �ƣ�void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����LCD_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void LCD_PutChar(u16 startx,u16 starty,u8 c,u16 charColor,u16 bkColor)  // Lihao
{
	u16 i=0;
	u16 j=0;	
	u8 tmp_char=0;

	if(HyalineBackColor == bkColor)
	{
		for (i=0;i<16;i++)
		{
			tmp_char=ascii_8x16[((c-0x20)*16)+i];
			for (j=0;j<8;j++)
			{
				if ( ((tmp_char >> (7-j)) & 0x01) == 0x01)
				{
					POINT_SET(startx+j, starty+i,charColor);
				}
				else
				{
					// do nothing // ͸������
				}
			}
		}
	}
	else
	{
		for (i=0;i<16;i++)
		{
			tmp_char=ascii_8x16[((c-0x20)*16)+i];
			for (j=0;j<8;j++)
			{
				if ( ((tmp_char >> (7-j)) & 0x01) == 0x01)
				{
					POINT_SET(startx+j, starty+i,charColor);
				}
				else
				{
					POINT_SET(startx+j, starty+i,bkColor);
				}
			}
		}
	}
}

/****************************************************************************
* ��    �ƣ�void LCD_Clear(u16 dat)
* ��    �ܣ�����Ļ����ָ������ɫ��������������� 0xffff
* ��ڲ�����dat      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Clear(0xffff);
****************************************************************************/
void LCD_Clear(u32 Color)
{
	int x,y;

	for(x = 0; x<vinfo.xres; x++)
	{
		for(y=0; y<vinfo.yres; y++)
		{
			POINT_SET(x,y,Color);
		}
	}
}
void draw_color(int length,int width ,int color)
{	
	int x,y,count =0;	
	for(x = 0;x < length ; x++)
	{		
		for(y = 0; y < width ; y++)
		{			
			*( fb_buf + y*length + x) = color;		
		}	
	}
}
void draw_line(int color,int width)
{	
	int x;	
	for(x = 0; x <= LENGTH ; x++)
	{
		*( fb_buf + width*LENGTH + x) = color;
	}
}


/****************************************************************************
* ��    �ƣ�void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��16x24�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����LCD_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void LCD_PutChar_16x24(u16 startx,u16 starty,u8 c,u16 charColor,u16 bkColor)
{

	u16 i=0;
	u16 j=0;
	
	u16 tmp_char=0;
	
	if(HyalineBackColor == bkColor)
	{
		for (i=0;i<24;i++)
		{
			tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
			for (j=0;j<16;j++)
			{
				if (((tmp_char >> j) & 0x01) == 0x01)
				{
					POINT_SET(startx+j, starty+i,charColor);
				}
				else
				{
					// do nothing // ͸������
				}
			}
		}

  	}
	else
	{
		for (i=0;i<24;i++)
		{
			tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
			for (j=0;j<16;j++)
			{
				if ( ((tmp_char >> j) & 0x01) == 0x01)
				{
					POINT_SET(startx+j, starty+i,charColor);
				}
				else
				{
					POINT_SET(startx+j, starty+i,bkColor);
				}
			}
		}
	}
}

/****************************************************************************
* ��    �ƣ�void LCD_Put16x16(u16 x,
                                  u16 y,
								  u8 g[2],
								  u16 fColor,
								  u16 bColor)
* ��    �ܣ���ָ��λ��д��16x16�ĺ���
* ��ڲ�����u16 x,       ������
            u16 y,		 ������
		    u8 g[2],	 ���ֱ���
		    u16 fColor,	 ǰ��ɫ
		    u16 bColor)	 ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Put16x16(0,0,���ҡ�, Black,White)��
****************************************************************************/

void LCD_Put16x16(u16 startx,u16 starty,const char g[3],u16 fColor,u16 bColor)
{ 
	FILE *fp;
	char buffer[32];     //buffer�����洢һ������
	unsigned int i,j;
	int qh,wh;
	long offset;

	if ((fp=fopen("/chinese/HZK16","rb"))==NULL)       //�򿪺��ֿ⣬���ֿ������ucdos���ҵ�
	{ 
		printf("Can't open HZK16,Please add it");
		exit(EXIT_FAILURE);
	}

	qh=	(g[0] - 0xa1); //������λ��
	wh= (g[1] - 0xa1);

	
	offset=(94*qh + wh)*32;   //����ú������ֿ���ƫ����
	

	fseek(fp,offset,SEEK_SET);
	fread(buffer,32,1,fp); //ȡ������32�ֽڵĵ�����ģ����buffer�У�һ�����֣�

	
	for(i=0;i<16;i++)                                   //�����ȣ������ҵ�����ʾ��ģλ�ã�ѭ��д��32�ֽ�
	{
		u8 m=buffer[i<<1];                 //��ȡ32�ֽ��еĵ�i�ֽ�
		for(j=0;j<8;j++)                                  //ѭ��д��8λ����
		{                                                
			if((m&0x80)==0x80) 	POINT_SET(startx+j, starty+i,fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
			else              	POINT_SET(startx+j, starty+i,bColor);      //���λΪ0��д�뱳����ɫ
			m<<=1;                                         //����1λ��׼��д��һλ
		} 

		m=buffer[(i<<1)+1];                 //��ȡ32�ֽ��еĵ�i�ֽ�
		for(j=0;j<8;j++)                                  //ѭ��д��8λ����
		{                                                
			if((m&0x80)==0x80)	POINT_SET(startx+8+j, starty+i,fColor);      //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
			else              	POINT_SET(startx+8+j, starty+i,bColor);      //���λΪ0��д�뱳����ɫ
			m<<=1;                                         //����1λ��׼��д��һλ
		}
	}
	return ;
}

#if 0
/****************************************************************************
* ��    �ƣ�void LCD_Put32x32(u16 x,
                                  u16 y,
								  u8 g[2],
								  u16 fColor,
								  u16 bColor)
* ��    �ܣ���ָ��λ��д��32x32�ĺ���
* ��ڲ�����u16 x,       ������
            u16 y,		 ������
		    u8 g[3],	 ���ֱ���
		    u16 fColor,	 ǰ��ɫ
		    u16 bColor)	 ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Put32x32(0,0,���ҡ�, Black,White)��
****************************************************************************/

void LCD_Put32x32(u16 startx,
                  u16 starty,
				  const u8 g[3],
				  u16 fColor,
				  u16 bColor)
{
	u8 i,j,k;
	

	u16 x = 208 - starty;   //x = 239 - (starty + 31)
	u16 y = startx;         //y = startx

	LCD_WR_CMD(0x0050, x); 	   //ˮƽ��ʾ����ʼ��ַ 0-239
	LCD_WR_CMD(0x0051, x+31);   //ˮƽ��ʾ��������ַ 0-239
	LCD_WR_CMD(0x0052, y);     //��ֱ��ʾ����ʼ��ַ 0-319
	LCD_WR_CMD(0x0053, y+31);  //��ֱ��ʾ��������ַ 0-319
	LCD_WR_CMD(32, x);		   //ˮƽ��ʾ����ַ
	LCD_WR_CMD(33, y);		   //��ֱ��ʾ����ַ	
	LCD_WR_REG(34);            //׼��д������ʾ��

	for (k=0;k<64;k++)  //���64����������                                     //ѭ��64�Σ���ѯ������ģλ��
	{ 
		if ((ch32[k].GBK[0]==g[0])&&(ch32[k].GBK[1]==g[1]))   //�жϵ�k�����ֵı����Ƿ������뺺��g[2]���
		{ 
			for(i=0;i<32;i++)                                   //�����ȣ������ҵ�����ʾ��ģλ�ã�ѭ��д��32�ֽ�
			{
				u8 m=ch32[k].hz32[i*4];                 //��ȡ32�ֽ��еĵ�i�ֽ�
				for(j=0;j<8;j++)                                  //ѭ��д��8λ����
				{                                                
					if((m&0x80)==0x80) 	LCD_WR_Data(fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
					else              	LCD_WR_Data(bColor);      //���λΪ0��д�뱳����ɫ
					m<<=1;                                         //����1λ��׼��д��һλ
				} 
				
				m=ch32[k].hz32[i*4+1];                 //��ȡ32�ֽ��еĵ�i�ֽ�
				for(j=0;j<8;j++)                                  //ѭ��д��8λ����
				{                                                
					if((m&0x80)==0x80)	LCD_WR_Data(fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
					else              	LCD_WR_Data(bColor);      //���λΪ0��д�뱳����ɫ
					m<<=1;                                         //����1λ��׼��д��һλ
				}

				m=ch32[k].hz32[i*4+2];                 //��ȡ32�ֽ��еĵ�i�ֽ�
				for(j=0;j<8;j++)                                  //ѭ��д��8λ����
				{                                                
					if((m&0x80)==0x80)	LCD_WR_Data(fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
					else              	LCD_WR_Data(bColor);      //���λΪ0��д�뱳����ɫ
					m<<=1;                                         //����1λ��׼��д��һλ
				}

				m=ch32[k].hz32[i*4+3];                 //��ȡ32�ֽ��еĵ�i�ֽ�
				for(j=0;j<8;j++)                                  //ѭ��д��8λ����
				{                                                
					if((m&0x80)==0x80)	LCD_WR_Data(fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
					else              	LCD_WR_Data(bColor);      //���λΪ0��д�뱳����ɫ
					m<<=1;                                         //����1λ��׼��д��һλ
				}
			}
			return ;
		}  
	}	
}
#endif

#if 0
/****************************************************************************
* ��    �ƣ�void LCD_Put24x24(u16 x,
                                  u16 y,
								  u8 g[3],
								  u16 fColor,
								  u16 bColor)
* ��    �ܣ���ָ��λ��д��24x24�ĺ���
* ��ڲ�����u16 x,       ������
            u16 y,		 ������
		    u8 g[3],	 ���ֱ���
		    u16 fColor,	 ǰ��ɫ
		    u16 bColor)	 ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Put24x24(0,0,���ҡ�, Black,White)��
****************************************************************************/

void LCD_Put24x24(u16 startx,
                  u16 starty,
				  const char g[3],
				  u16 fColor,
				  u16 bColor)
{ 
	FILE *fp;
	char buffer[72];     //buffer�����洢һ������
	unsigned int i,j;
	int qh,wh;
	unsigned long offset;

	if ((fp=fopen("/chinese/HZK24","rb"))==NULL)       //�򿪺��ֿ⣬���ֿ������ucdos���ҵ�
	{ 
		printf("Can't open HZK24,Please add it");
		exit(EXIT_FAILURE);
	}

	qh=	(g[0] - 0xa1); //������λ��
	wh= (g[1] - 0xa1);

	
	offset=(94*qh + wh)*72;   //����ú������ֿ���ƫ����
	

	fseek(fp,offset,SEEK_SET);
	fread(buffer,72,1,fp); //ȡ������32�ֽڵĵ�����ģ����buffer�У�һ�����֣�

	
	for(i=0;i<24;i++)                                   //�����ȣ������ҵ�����ʾ��ģλ�ã�ѭ��д��32�ֽ�
	{
		u8 m=buffer[i*3];                 //��ȡ32�ֽ��еĵ�i�ֽ�
		for(j=0;j<8;j++)                                  //ѭ��д��8λ����
		{                                                
			if((m&0x80)==0x80) 	POINT_SET(startx+j, starty+i,fColor);     //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
			else              	POINT_SET(startx+j, starty+i,bColor);      //���λΪ0��д�뱳����ɫ
			m<<=1;                                         //����1λ��׼��д��һλ
		} 

		m=buffer[(i*3)+1];                 //��ȡ32�ֽ��еĵ�i�ֽ�
		for(j=0;j<8;j++)                                  //ѭ��д��8λ����
		{                                                
			if((m&0x80)==0x80)	POINT_SET(startx+8+j, starty+i,fColor);      //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
			else              	POINT_SET(startx+8+j, starty+i,bColor);      //���λΪ0��д�뱳����ɫ
			m<<=1;                                         //����1λ��׼��д��һλ
		}
		
		m=buffer[(i*3)+2];                 //��ȡ32�ֽ��еĵ�i�ֽ�
		for(j=0;j<8;j++)                                  //ѭ��д��8λ����
		{                                                
			if((m&0x80)==0x80)	POINT_SET(startx+16+j, starty+i,fColor);      //�ж����λ�Ƿ�Ϊ1,���λΪ1��д���ַ���ɫ
			else              	POINT_SET(startx+16+j, starty+i,bColor);      //���λΪ0��д�뱳����ɫ
			m<<=1;                                         //����1λ��׼��д��һλ
		}
	}
	return ;
}
#endif


//================================================================================================
//	ʵ�ֹ��ܣ�	��ʾ��Ӣ���ַ���
//  ���������  x ������
//              y ������
//		        *s ����ʾ���ַ���,����LCD_PutString24x24(24,16,"123��о",White,Blue);����"123��о"�ĵ�һ���ַ���ַ����ָ�����s.
//		        bColor �ַ�������ɫ
//================================================================================================
void LCD_PutString24x24(u16 x,u16 y,const char *s,u16 fColor,u16 bColor) 
{
	u8 l=0;                            //��ʾ��λ������
	while(*s) 
	{
		if( *s < 0x80) //�ж�sָ����ַ����е�ĳ�ַ��ı���ֵ�Ƿ�С��128,���С��,��ΪASCII�ַ�
	    {
			LCD_PutChar_16x24(x+l*8,y,*s,fColor,bColor);//��ʾ���ַ�
	    	s++;l+=2;                              //ָ���1,λ�ü�1
		}
		else
	    {
//			LCD_Put24x24(x+l*8,y,s,fColor,bColor);//��ʾ�ú���
	    	s+=2;l+=3;          //��Ϊ����Ϊ����Ϊ2�ֽڣ�ָ���2,��ʾ16x16����λ�ü�2
		}
	}
}


//================================================================================================
//	ʵ�ֹ��ܣ�	��ʾ��Ӣ���ַ���
//  ���������  x ������
//              y ������
//		        *s ����ʾ���ַ���,����LCD_PutString(24,16,"123��о",White,Blue);����"123��о"�ĵ�һ���ַ���ַ����ָ�����s.
//		        bColor �ַ�������ɫ
//================================================================================================
void LCD_PutString(u32 x,u32 y,const char *s,u32 fColor,u32 bColor) 
{
	u8 l=0;                            //��ʾ��λ������
	while(*s) 
	{
		if( *s < 0x80) //�ж�sָ����ַ����е�ĳ�ַ��ı���ֵ�Ƿ�С��128,���С��,��ΪASCII�ַ�
	    {
			LCD_PutChar(x+l*8,y,*s,fColor,bColor);//��ʾ���ַ�
	    	s++;l++;                              //ָ���1,λ�ü�1
		}
		else
	    {
			LCD_Put16x16(x+l*8,y,s,fColor,bColor);//��ʾ�ú���
	    	s+=2;l+=2;          //��Ϊ����Ϊ����Ϊ2�ֽڣ�ָ���2,��ʾ16x16����λ�ü�2
		}
	}
}	


/****************************************************************************
* ��    �ƣ�void LCD_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
* ��    �ܣ���ָ�����껭ֱ��
* ��ڲ�����x0     A��������
*           y0     A��������
*           x1     B��������
*           y1     B��������
*           color  ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����LCD_Line(0,0,240,320,0x0000);
****************************************************************************/
void LCD_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
			POINT_SET(x,y0,color);

   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			POINT_SET(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			POINT_SET(x,y,color);
  		}
 	}
 	else
 	{
 		POINT_SET(x,y,color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				POINT_SET(x,y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				POINT_SET(x,y,color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				POINT_SET(x,y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				POINT_SET(x,y,color);
    			}
   			}
  		}
 	}
}


/****************************************************************************
* ��    �ƣ�void LCD_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* ��    �ܣ���ָ�����껭Բ�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void LCD_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			LCD_Line(cx+x,cy+y,cx-x,cy+y,color);
			LCD_Line(cx+x,cy-y,cx-x,cy-y,color);
			LCD_Line(cx+y,cy+x,cx-y,cy+x,color);
			LCD_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			POINT_SET(cx+x,cy+y,color);
			POINT_SET(cx-x,cy+y,color);
			POINT_SET(cx+x,cy-y,color);
			POINT_SET(cx-x,cy-y,color);
			POINT_SET(cx+y,cy+x,color);
			POINT_SET(cx-y,cy+x,color);
			POINT_SET(cx+y,cy-x,color);
			POINT_SET(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}


/****************************************************************************
* ��    �ƣ�void LCD_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
* ��    �ܣ���ָ�����򻭾��Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void LCD_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
	if(fill)
	{
		u16 i;
		if(x0>x1)
		{
			i=x1;
			x1=x0;
		}
		else
		{
			i=x0;
		}
		for(;i<=x1;i++)
		{
			LCD_Line(i,y0,i,y1,color);
		}
		return;
	}
	LCD_Line(x0,y0,x0,y1,color);
	LCD_Line(x0,y1,x1,y1,color);
	LCD_Line(x1,y1,x1,y0,color);
	LCD_Line(x1,y0,x0,y0,color);
}


/****************************************************************************
* ��    �ƣ�void  LCD_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
* ��    �ܣ���ָ�����������Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void  LCD_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
	LCD_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}

static struct bmp_info	*bmp_get_info(const char *path)
{
	int	fd;
	int	size;
	int	palette_size;
	struct	bmp_info *pinfo;

	pinfo = (struct bmp_info*)malloc(sizeof(struct bmp_info));
	if (NULL == pinfo)
	{
		printf("bmp_get_info: malloc bmp_info failed...\n");
	}

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("bmp_get_info: Invalid fd...\n");
		free(pinfo);
		return NULL;
	}

	// read file header
	size = read(fd, (char*)(&pinfo->file_head), sizeof(struct bmp_file_header));
	if (size != sizeof(struct bmp_file_header))
	{
	}
	//printf("%d\n", sizeof(unsigned short));
	//printf("File header size: %d\n", size);
	printf("\n\n");
	printf("--------------- File Header Information-------------------\n");
	printf("File name           : %s\n", path);
	printf("File type           : %c%c\n", pinfo->file_head.bf_type &0xff, pinfo->file_head.bf_type >> 8);
	printf("File size           : %d\n", pinfo->file_head.bf_size);
	printf("Data offset         : %d\n", pinfo->file_head.offset);
	
	// read info header
	size = read(fd, (char*)(&pinfo->info_head), sizeof(struct bmp_info_header));
	if (size != sizeof(struct bmp_file_header))
	{
	}
	printf("\n\n");
	printf("--------------- Info Header Information-------------------\n");
	printf("Info header size    : %d\n", pinfo->info_head.header_size);
	printf("Image width         : %d pixel\n", pinfo->info_head.width);
	printf("Image height        : %d pixel\n", pinfo->info_head.height);
	printf("Image planes        : %d \n", pinfo->info_head.planes);
	printf("Bits per pixel      : %d \n", pinfo->info_head.bpp);
	printf("Image compression   : %d \n", pinfo->info_head.compression);
	printf("Image size          : %d \n", pinfo->info_head.image_size);
	printf("X pixel per meter   : %d \n", pinfo->info_head.xppm);
	printf("Y pixel per meter   : %d \n", pinfo->info_head.yppm);
	printf("Color used          : %d \n", pinfo->info_head.color_used);
	printf("Important color     : %d \n", pinfo->info_head.color_important);
	printf("\n\n");
	
	// palette
	palette_size 	= pinfo->file_head.offset - sizeof(struct bmp_file_header) - pinfo->info_head.header_size;
	printf("Palette size        : %d \n", palette_size);
	pinfo->palette 	= (unsigned int*)malloc(palette_size / sizeof(int));
	if (NULL == pinfo->palette)
	{
	}
	size = read(fd, (unsigned char*)pinfo->palette, palette_size);
	if (size != palette_size)
	{
	}

	// bmp data
	pinfo->pdat = (unsigned char*)malloc(pinfo->info_head.image_size);
	if (NULL == pinfo->pdat)
	{
		printf("Malloc image data failed\n");
	}
	size = read(fd, (unsigned char*)pinfo->pdat, pinfo->info_head.image_size);
	if (pinfo->info_head.image_size != size)
	{
		printf("Read image data failed\n");
	}
	close(fd);

	return pinfo;
}

static void bmp_draw(u16 startx, u16 starty, struct bmp_info *pinfo, unsigned char *pfb)
{
	unsigned short	r;
	unsigned short	g;
	unsigned short	b;
	unsigned short	bpp;
	unsigned char	*pdat;

	int x,y;
	
	if (NULL == pinfo)
	{
		printf("pinfo...\n");
	}
	if (NULL == pfb)
	{
		printf("pfb...\n");
	}
	bpp	= pinfo->info_head.bpp;
	pdat	= pinfo->pdat;

	switch (bpp)
	{
		case BPP_1:
			break;
		case BPP_4:
			break;
		case BPP_8:
			break;
		case BPP_16:
			break;
		case BPP_24:

			if (pinfo->info_head.height > 0) {	// bottom->top
				for(y=pinfo->info_head.height-1; y>=0; y--)
				{
					for(x=0; x<pinfo->info_head.width; x++)
					{
						b	= (unsigned short)(*pdat ++);
						g	= (unsigned short)(*pdat ++);
						r	= (unsigned short)(*pdat ++);
						//gui_set_pixel(i % vinfo.xres, vinfo.yres - i / vinfo.xres - 1, 
						//	(r << 11) | (g << 5) | b);					
						POINT_SET(x + startx, y + starty,((r << 8)&(0xf800)) | ((g << 3)&(0x7e0)) | ((b>>3)&0x1f));					
					}
				}
			} else {				// top down
				for(y=0; y<pinfo->info_head.height; y++)
				{
					for(x=0; x<pinfo->info_head.width; x++)
					{
						b	= *pdat ++;
						g	= *pdat ++;
						r	= *pdat ++;
						//gui_set_pixel(i % vinfo.xres, vinfo.yres - i / vinfo.xres - 1, 
						//	(r << 11) | (g << 5) | b);					
						//POINT_SET(x + startx, y + starty,(r << 11) | (g << 5) | b);					
						POINT_SET(x + startx, y + starty,((r << 8)&(0xf800)) | ((g << 3)&(0x7e0)) | ((b>>3)&0x1f));					
					}
				}
			}

			break;
		case BPP_32:
			break;
	}	
}




/****************************************************************************
* ��    �ƣ�void LCD_DrawPicture(u16 StartX,u16 StartY,u16 High,u16 Wide,u8 *pic)* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           u16 High   ͼƬ�߶�
			u16 Wide   ͼƬ���
            pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
* ���÷�����ili9320_DrawPicture(0,0,100,100,(u16*)demo);
****************************************************************************/
void LCD_DrawPicture(u16 StartX,u16 StartY, const char * path)
{
#if 0
	u32 x,y;
	printf("DrawPicture\n");

	for(x=0; x < Wide; x++)
	{
		for(y=0; y<High; y++)
		{
			POINT_SET(StartX+x,StartY+y,pic[(x*y)*2]|(((u16)pic[x*y*2+1]) << 8));
		}
		
	}
#endif 
	
	struct bmp_info	*pinfo;
	
	pinfo = bmp_get_info(path);
	bmp_draw(StartX, StartY, pinfo, fb_buf);
}

