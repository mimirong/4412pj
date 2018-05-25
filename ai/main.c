#include"pwm.h"
#include <stdio.h>
#include "led.h"
#include <unistd.h> 
#include <stdlib.h>
#include"lcd.h"
#include"bmp.h"
#include"lcd_show_jpeg.h"

void beep(void);
void led(void);
void music(void);
void menu(void);
int voice(void);
int deal(int value);
void LCD(void);
void uart_init(void);
void volume(void);
void welcome(void);



int main(int argc, const char *argv[])
{
	int value;
	
	led_init();
	LCD_Init();
	pwm_init();
	uart_init();
	welcome();
/*	volume();
	//kai  ji  jie  mian  
	//mima  denglu

	while(1)
	{
		menu();
		value=voice();
		deal(value);
	}*/

	//beep();

	led_exit();
	pwm_exit();
	return 0;

}
//kaiji  jiemian
void welcome(void)
{
	int value;
	int c;
	show_jpeg("/pic/test.jpeg");
	sleep(1);
	//LCD_DrawPicture(0,0, "/pic/1.bmp");
	pwm_setfre(698);
	beep_on();
	sleep(1);
	beep_off();
	printf("请输入密码：");
	scanf("%d",&c);
	
	if (c==1) {
	volume();
	while(1)
		{
		menu();
		value=voice();
		deal(value);
		}

		
	}
	else
	{
		printf("密码错误");
		exit(0);
	}
	
	
}

void LCD(void)
{	LCD_Clear(Green);
	sleep(1);
	LCD_PutChar(20,20,'A',Blue,Black);
	sleep(1);
	LCD_DrawPicture(0,0, "/pic/1.bmp");
	sleep(1);
	LCD_Clear(Black);	//LCD_DrawPicture(0,0, "/pic/2.bmp");
	sleep(5);
	LCD_PutString(0,1,   "                _._",						White,Black);
	LCD_PutString(0,28,  "           _.-``__ ''-._",				White,Black);
	LCD_PutString(0,56,  "      _.-``    `.  `_.  ''-._           ",White,Black);
	LCD_PutString(0,84,  "  .-`` .-```.  ```\/    _.,_ ''-._",		White,Black);
	LCD_PutString(0,112, "  (    '      ,       .-`  | `,   )    ",White,Black);
	LCD_PutString(0,140, " |`-._`-...-` __...-.``-._|'` _.-'|    ",White,Black);
	LCD_PutString(0,168, " |    `-._   `._    /     _.-'    |     ",White,Black);
	LCD_PutString(0,196, " |    `-._   `._    /     _.-'    |     ",White,Black);
	LCD_PutString(0,224, "  `-._    `-._  `-./  _.-'    _.-'", 	   White,Black);
	LCD_PutString(0,252, " |`-._`-._    `-.__.-'    _.-'_.-'|",	   White,Black);
	LCD_PutString(0,280, " |    `-._`-._        _.-'_.-'    |           ",White,Black);
	LCD_PutString(0,308, " |    `-._`-._        _.-'_.-'    |           ",White,Black);
	LCD_PutString(0,336, " |    `-._`-._        _.-'_.-'    |           ",White,Black);
	LCD_PutString(0,364, "  `-._    `-._`-.__.-'_.-'    _.-'",			White,Black);
	LCD_PutString(0,392, " |`-._`-._    `-.__.-'    _.-'_.-'|",			White,Black);
	LCD_PutString(0,420, " |    `-._`-._        _.-'_.-'    |",White,Black);
	LCD_PutString(0,448, "  `-._    `-._`-.__.-'_.-'    _.-'",White,Black);
	LCD_PutString(0,476, "  `-._    `-._`-.__.-'_.-'    _.-'",White,Black);
	LCD_PutString(0,504, "      `-._    `-.__.-'    _.-'",White,Black);
	LCD_PutString(0,532, "          `-._        _.-'",White,Black);
	LCD_PutString(0,560, "              `-.__.-'  ",White,Black);






	
}

/*   
                


 





 */


void menu(void)
{
	printf("plses Input a number:\n");
	printf("1-----------led on\n");
	printf("2-----------led off\n");
	printf("3-----------beep_on\n");
	printf("4-----------beep_off\n");
	printf("5-----------music\n");
	printf("6-----------picture\n");
	printf("7-----------vidoes\n");
	printf("8-----------exit\n");
	/*	printf("9-----------led on\n");
		printf("10-----------led on\n");
		printf("11-----------led on\n");
		printf("12-----------led on\n");
		printf("13-----------led on\n");*/
}

/*int voice(void)
  {
  int value;
  printf("plses Input a number:\n");
  scanf("%d",&value);
  return value;
  }*/

int deal(int value)
{
	switch(value)
	{
	case 1: 
		led_on(1);
		led_on(2);
		break;

	case 2: 
		led_off(2);
		break;

	case 3: pwm_setfre(523);     //1 .523  2.587   3.659  4.698   5.783   6.880  7.987
		beep_on();
		break;
	case 4:beep_off();
		break;
	case 5: music();
			break;
	case 6: 
			LCD();
			break;
	case 7: show_jpeg("/pic/test.jpeg");
			break;
	case 8: printf("exit*************out\n");
			exit(0);
			break;

	case 9:
			break;
	}
	return 0;
}


int hz[3][8]={
	[0]=88888110,523,587,659,698,783,880,987,
	[1]=8000000,261,293,329,349,391,440,493,
	[2]=8000000,1046,1174,1318,1396,1567,1760,1975,};

unsigned char dahai []={13,15,16,16,16,16,21,16,15,15,16,15,
	13,12,11,11,11,11,12,13};

unsigned char time[]={8,8,8,4,8,4,8,8,8,4,8,4,
	8,8,8,4,8,4,4,4,};

#define FRE 2500000
void music(void)
{
	int a,b;
	int i;
	int t;
	for(i=0; i<sizeof(dahai);i++)

	{
		a=dahai[i]/10;
		b=dahai[i]%10;
		pwm_setfre(hz[a][b]);     //1 .523  2.587   3.659  4.698   5.783   6.880  7.987
		beep_on();
		usleep(FRE/time[i]);
		beep_off();
		usleep(100);
	}


}





void beep(void)
{
		pwm_setfre(523);     //1 .523  2.587   3.659  4.698   5.783   6.880  7.987
		beep_on();
		sleep(5);
		beep_off();
		sleep(5);
	
}






void led(void)
{
	while(1)
	{
		led_on(1);
		sleep(1);
		led_off(1);

		//sleep(1);
		led_on(2);
		sleep(1);
		led_off(2);

		//sleep(1);
		led_on(3);
		sleep(1);
		led_off(3);


		//sleep(1);
		led_on(4);
		sleep(1);
		led_off(4);
		//sleep(1);
	}
}

