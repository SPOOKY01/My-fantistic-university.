#include<reg52.h>//79
sbit --------;
unsigned char code LedChar[] = 
{
    0xC0,0xF9,oxA4,0xB0,0x99,0x92,0x82,0xF8,
    0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,
};
unsigned char LedBuff[6] = 
{
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
unsigned char i = 0;
unsigned int cnt = 0;
unsigned char flag1s = 0;
void main()
{
    unsigned long sec = 0;
    EA = 1;
    ENLED = 0;
    ADDR3 = ?;
    TMOD = 0x01;
    TH0 = 0xFC;
    TL0  = 0x67;
    ET0 = 1;
    TR0 = 1;
    while(1)
    {
        if(flag1s == 1)
        {
            flag1s = 0;
            sec++;
            LedBuff[0] = LedChar[sec%10]；
            LedBuff[1] = LedChar[sec/10%10]；
            LedBuff[2] = LedChar[sec/100%10]；
            LedBuff[3] = LedChar[sec/1000%10]；
            LedBuff[4] = LedChar[sec/10000%10]；
            LedBuff[5] = LedChar[sec/100000%10]；
        }
    }
}
//
void InterruptTime0() iterrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x67;
    cnt++;
    if(cnt >= 1000)
    {
        cnt = 0;
        flag1s = 1;
    }
    //
    PO = 0xFF
    switch(i)
    {
        case0:ADDR2 = 0;ADDR1 = 0;ADDRO = 0;i++;P0 = LedBuff[0];break;
        case1:ADDR2 = 0;ADDR1 = 0;ADDRO = 1;i++;P0 = LedBuff[1];break;
        case2:ADDR2 = 0;ADDR1 = 1;ADDRO = 0;i++;P0 = LedBuff[2];break;
        case3:ADDR2 = 0;ADDR1 = 1;ADDRO = 1;i++;P0 = LedBuff[3];break;
        case4:ADDR2 = 1;ADDR1 = 0;ADDRO = 0;i++;P0 = LedBuff[4];break;
        case5:ADDR2 = 1;ADDR1 = 0;ADDRO = 1;i=0;P0 = LedBuff[5];break;
        default:break;
    }
}




#include<reg52.h>//145

sbit BUZZ = P1^6;
unsigned char T0RH = 0;
unsigned char T0RL = 0;

void OpenBuzz(unsigned int frequ);
void StopBuzz()

void mian()
{
    unsigned int i;

    TMOD = 0x01;
    EA = 1;

    while(1)
    {
        OpenBuzz(4000);
        for(i = 0; i<40000; i++);
        StopBuzz();
        for(i = 0; i<40000; i++);    
        OpenBuzz(1000);
        for(i = 0; i<40000; i++);
        StopBuzz();
        for(i = 0; i<40000; i++);
    }
}
/*蜂鸣器启动函数*/
void OpenBuzz(unigned int frequ)
{
    unsigned int reload;

    reload = 65536 - (110592/12)/(frequ*2);
    T0RH = (unsigned char)(reload>>8);
    T0RH = (unsigned char)reload;
    TH0 = 0xFF;
    TL0 = 0xFF;
    ET0 = 1;
    TR0 = 1;
}
/*蜂鸣器停止函数*/
void StopBuzz()
{
    ET0 = 0;
    TR0 = 0;
}
/*~~~*/
void InterruptTimer0() interrupt 1
{
    TH0 = TORH;
    TL0 = TORL;
    BUZZ = ~BUZZ;
}
#include<reg52.h>//179
sbit PIN_RXD = P3^0;//接收引脚定义
sbit PIN_TXD = P3^1;//发送引脚定义

bit RxdOrTxd = 0;//指示当前状态为接受还是发送
bit RxdEnd = 0;//接收结束标志
bit TxdEnd = 0;//发送结束标志
unsigned char RxdBuf = 0;//接收缓冲器
unsigned char TxdBuf = 0;//发送缓冲器

void ConfigUART(unsigned int band);
void StartTXD(unsigned char dat);
void StartRXD();

void main()
{
    EA = 1;
    ConfigUART(9600);

    while(1)
    {
        while(PIN_RXD);
        StartRXD();
        while(!RxdEnd);
        StartTXD(RxdBuf + 1);
        while(!TxdEnd);
    }
}
/*串口配置函数。。。*/
void ConfigUART(unsigned int baud)
{
    TMOD &= 0xF0;
    TMOD |= 0x02;
    TH0 = 256 - (11059200/12)/baud;
}
/*启动串行发送*/
void StartRXD()
{
    TL0 = 256 - ((256 - TH0)>>1);
    ET0 = 1;
    TR0 = 1;
    RxdEnd = 0;
    RxdOrTxd = 0;
}
/*启动串行发送*/
void StartTXD(unsigned char dat)
{
    TxdBuf = dat;
    TL0 = TH0;
    ET0 = 1;
    TR0 = 1;
    PIN_TXD = 0;
    TxdEnd = 0;
    RxdOrTxd = 1;
}
/*中断服务函数，处理串行发送和接收*/
void InterruptTimer0() interrupt1
{
    static unsigned char cnt = 0;

    if(RxdOrTxd)
    {
        cnt++;
        if(cnt <= 8)
        {
            PIN_TXD = TxdBuf & 0x01;
            TxdBuf >>= 1;
        }
        else if(cnt == 9)
        {
            PIN_TXD = 1;
        }
        else
        {
            cnt = 0;
            TR0 = 0;
            TxdEnd = 1;
        }
    }
    else
    {
        if(cnt == 0)
        {
            if(!PIN_TXD)
            {
                RxdBuf = 0;
                cnt++;
            }
            else
            {
                TR0 = 0;
            }
        }
        else if(cnt <= 8)
        {
            RxdBuf >>=1;
            if(PIN_RXD)
            {
                RxdBuf |= 0x80;
            }
            cnt++;
        }
        else
        {
            cnt = 0;
            TR0 = 0;
            if(PIN_RXD)
            {
                RxdEnd = 1;
            }
        }
    }
}
#icnlude<reg52.h>//184

void ConfigUART(unsigned int baud);

void main()
{
    ConfigUATR(9600)//

    while(1)
    {
        while(!RI);//
        RI = 0;
        SBUF = SBUF + 1;
        while(!TI);
        TI = 0;
    }
}
//
void ConfigUART(unsigned int baud)
{
    SCON = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0X20;
    TH1 = 256 - (11059200/12/32)/baud;
    TL1 = TH1;
    ET1 = 0;
    TR1 = 1;
}

#include<reg52.c>//185

void ConfigUART(unsigned int baud)

void main()
{
    EA = 1;//
    ConfigUART(9600);//配置波特
    while(1);
}
//
void ConfigUART(unsigned int baud)
{
    SCON = 0x50;//
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 256 - (11059200/12/32)/baud;
    TL1 = TH1;
    ET1 = 0;
    ES = 1;
    TR1 = 1;
}
//
void InterruptUART() interrput 4
{
    if(RI)
    {
        RI = 0;
        SBUF = SBUF + 1;
    }
    if(TI)
    {
        TI = 0;
    }
}




#include<reg52.h>//209
#define LCD1602_DB P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

void IntLcd1602();
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main()
{
    unsigned char str[] = "Kingst Studio";

    InitLcd1602();
    LcdShowStr(2, 0, str);
    LcdShouStr(0, 1, "Welcome to EDD");
    while(1);
}
//等待液晶准备好
void LcdWaitReady()
{
    unsigned char sta;

    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_E = 1;
        sta = LCD1602_DB;
        LCD1602_E = 0;
    }
    while(sta & 0x80);
}
//
void LcdWriteCmd(unsigned char amd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
//
void LcdWriteDat(unsigned char dat)
{
    LcdWriteReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
//
void LcdSetCursor(unsiged char x, unsigned char y)
{
    unsigned char addr;

    if(y == 0)
    {
        addr = 0x00 + x;
    }
    else
    {
        addr = 0x40 + x;
    }
    LcdWriteCmd(addr | 0x80);
}
//
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
    LcdSetCursor(x, y);
    while(*str != '\0')
    {
        LcdWriteDat(* str++);
    }
}
//初始化LCD1602
void InitLcd1602()
{
    LcdWriteCmd(0x38);
    LcdWriteCmd(0x0C);
    LcdWriteCmd(0x06);
    LcdWriteCmd(0x01);
}




#include<reg52.h>//215

#define LCD1602_DB P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

bit flag500ms = 0;
unsigned char T0RH = 0;
unsigned char T0RL = 0;
//
unsigned char code str2[] = "MEOWME OWMEOW";
//
unsigned char code str2[] = "Let's move\\\";

void ConfigTimer0(unsigned int ms);
void InitLcd1602();
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str, unsigned char len);

void main()
{
    unsigned char i;
    unsigned char index = 0;
    unsigned char pdat bufMovel[16+sizeof(str1) + 16];
    unsigned char pdat bufMove2[16+sizeof(str1) + 16];
    
    EA = 1;
    ConfigTimer0(10);
    IntLcd1602();
    //
    for(i = 0; i<16; i++)
    {
        bufMove1[i] = ' ';
        bufMove2[i] = ' ';
    }
    //
    for(i = 0;i<(sizeof(str1) - 1); i++)
    {
        bufMove1[16+i] = str1[i];
        bufMove2[16+i] = str2[i];
    }
    //
    for(i = (16 + sizeof(str1) - 1); i<sizeof(bufMove1); i++)
    {
        bufMove1[i] = '';
        bufMove2[i] = '';
    }
    
    while(1)
    {
        if(flag500ms)
        {
            flag500ms = 0;
            //
            LcdShowStr(0, 0, bufMove1 + index, 16);
            LcdShowStr(0, 1, bufMove2 + index, 16);
            //
            index++;
            if(index >= (16 + sizeof(str1) - 1))
            {
                //
                index = 0;
            }
        }
    }
}
//
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;
    
    tmp = 110592/12;
    tmp = (tmp * ms)/1000;
    tmp = 65536 - tmp;
    tmp = tmp + 12;
    T0RH = (unsigned char)(tmp>>8);
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;
    TR0 = 1;
}
void LcdWaitReady()
{
    unsigned char sta;
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_E = 1;
        sta = LCD1602_DB;
        LCD1602_E = 0;
    }
    while(sta & 0x80);
}
//
void LcdWriteCmd(unsigne char cmd)
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
//
void LcdWriteDat(unsigned char dat)
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}
void LcdSetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    if(y == 0)
    {
        addr = 0x00 + x;
    }
    else
    {
        addr = 0x40 + x;
    }
    LcdWriteCmd(adr | 0x80);
}
//
void LcdShowStr(unsigned char x, unsigned char y, unsigned char*str, unsigned char len)
{
    LcdSetCursor(x, y);
    while(len--)
    {
        LcdWriteDat( * str++);
    }
}
//
viod InitLcd1602()
{
    LcdWriteCmd(0x38);
    LcdWriteCmd(0x0C);
    LcdWriteCmd(0x06);
    LcdWriteCmd(0x01);
}
//
void InterruptTimer0 inturrupt 1
{
    static unsigned char tmr500ms = 0;
    
    TH0 = T0RH;
    TL0 = T0RL;
    tmr500ms++;
    if(tmr500ms >= 50)
    {
        tmr500ms = 0;
        flag500ms = 1;
    }
}

