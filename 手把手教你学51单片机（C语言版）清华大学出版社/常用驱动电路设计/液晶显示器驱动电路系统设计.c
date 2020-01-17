#include<reg52.h>  //包含头文件，一般情况无需改动
		           //头文件包含特殊功能寄存器的定义
#include<intrins.h>
sbit RS=P2^4;	   //定义端口
sbit RW=P2^5;
sbit EN=P2^6;
#define RS_CLR RS=0
#define	RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0
//μs延时函数
void DelayUs2x(unsigned char t)
{
	while(--t);
}
//ms延时函数
void DelayMs(unsigned char t)
{
	while(t--)
	{
		//???????1ms
		DelayUs2x(245);
		DelayUs2x(245);
	}
}
//判忙函数
bit LCD_Check_Busy(void)
	{
		DataPort = 0xFF;
		RS_CLR;
		RW_SET;
		EN_CLR;
		_nop_();
		EN_SET;
		return(bit)(DataPort & 0x80);
	}
//写入命令函数
void LCD_Write_Com(unsigned char com)
{
	//while(LCD_Check_Busy());//?????
	DelayMs(5);
	RS_CLR;
	RW_CLR;
	EN_SET;
	DataPort = com;
	_nop_(); 
	EN_CLR;
}
//写入数据函数
void LCD_Write_Data(unsigned char Data)
{
	//while(LCD_Check_Busy());//?????
	DelayMs(5);
	RS_SET;
	RW_CLR;
	EN_SET;
	DataPort = Data;
	_nop_();
	EN_CLR;
}
void LCD_Clear(void)
{
	LCD_Write_Com(0x01);
	DelayMs(5);
}
//写入字符串函数
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)
{
	if(y==0)
	{
		LCD_Write_Com(0x80 + x);//表示第一行
	}
	else
	{
		LCD_Write_Com(0xC0 + x);//表示第二行
	}
	while(*s)
	{
		LCD_Write_Data( *s);
		s++;
	}
}
//写入字符函数
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
{
	if(y==0)
	{
		LCD_Write_Com(0x80 + x);
	}
	else
	{
		LCD_Write_Com(0xC0 + x);
	}
	LCD_Write_Data(Data);
}
//初始化函数
void LCD_Init(void)
{
	LCD_Write_Com(0x38);/*显示模式设置*/
	DelayMs(5);
	LCD_Write_Com(0x38);
	DelayMs(5);
	LCD_Write_Com(0x38);
	DelayMs(5);
	LCD_Write_Com(0x38);
	LCD_Write_Com(0x08);//显示关闭
	LCD_Write_Com(0x01);//显示清屏
	LCD_Write_Com(0x06);//显示光标移动设置
	DelayMs(5);
	LCD_Write_Com(0x0C);//显示开以及光标设置
}
//主函数
void main(void)
{
	LCD_Init();
	LCD_Clear();//清屏
	LCD_Write_Char(7,0,0);
	LCD_Write_Char(8,0,0);
	LCD_Write_String(1,1,"www.baidu.com");
	while(1)
	{
		DelayMs(200000);
	}
}
