#ifndef _ILI9320_H_
#define _ILI9320_H_


#define POINT_SET(x,y,color) *((unsigned short*)fb_buf + (y) * vinfo.xres + (x)) = (color)
#define READ_SET(x,y)        (*((unsigned short*)fb_buf + (y) * vinfo.xres + (x)))

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define HyalineBackColor 0x0001  // Lihao Í¸Ã÷±³¾°


typedef unsigned char       u8;   
typedef unsigned short      u16;   
typedef unsigned int        u32;   
typedef long long           u64;   
typedef short               s16;

int LCD_Init(void);
void LCD_Clear(u16 Color);

void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void LCD_Put16x16(u16 startx,u16 starty,const char g[3],u16 fColor,u16 bColor);
void LCD_PutString(u16 x,u16 y,const char *s,u16 fColor,u16 bColor);

void LCD_PutChar_16x24(u16 startx,u16 starty,u8 c,u16 charColor,u16 bkColor);
//void LCD_Put24x24(u16 startx,u16 starty,const char g[3],u16 fColor,u16 bColor);
void LCD_PutString24x24(u16 x,u16 y,const char *s,u16 fColor,u16 bColor);


//void LCD_Put32x32(u16 startx,u16 starty,const char g[3],u16 fColor,u16 bColor);

void LCD_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
void LCD_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
void LCD_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill);
void LCD_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);

void LCD_DrawPicture(u16 StartX,u16 StartY, const char *path);

#endif
