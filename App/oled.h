#ifndef __OLED_H
#define __OLED_H			  	 

#include "stdlib.h"	    

		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_RST_Clr() PTB8_OUT=0
#define OLED_RST_Set() PTB8_OUT=1

#define OLED_RS_Clr() PTB9_OUT=0
#define OLED_RS_Set() PTB9_OUT=1

#define OLED_SCLK_Clr() PTB10_OUT=0
#define OLED_SCLK_Set() PTB10_OUT=1

#define OLED_SDIN_Clr() PTB11_OUT=0
#define OLED_SDIN_Set() PTB11_OUT=1

#define u8 unsigned char
#define u32 unsigned int


 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_Line(u8 Line);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 

void OLED_Cube(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_SelBar(unsigned char col);

void LCD_Display(unsigned char x,unsigned char y,long int num);//显示函数
void LCD_Display_float(unsigned char x,unsigned char y,float num);
void Display_CCD(void);

#endif  
	 



