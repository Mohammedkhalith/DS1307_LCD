// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000

#define rs RB0
#define rw RB1
#define en RB2
#define lcd PORTD


unsigned char sec, min, hour, day, date, month, year;


void lcd_init(void);
void cmd(unsigned char a);
void dat(unsigned char b);
void show(const char *s);

void rtc_init();
void rtc_start();
void rtc_stop();
void rtc_ack();
void rtc_nak();
void rtc_res();
void rtc_send(unsigned char a);
void rtc_send_byte(unsigned char addr, unsigned char data);
unsigned char rtc_read();
unsigned char rtc_read_byte(unsigned char addr);
void waitmssp();
unsigned char convup(unsigned char bcd);
unsigned char convd(unsigned char bcd);

void lcd_init()
{
    cmd(0x38);
    cmd(0x0C);
    cmd(0x06);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd = a;
    rs = 0; 
    rw = 0;
    en = 1;
    __delay_ms(2);
    en = 0;
}

void dat(unsigned char b)
{
    lcd = b;
    rs = 1; 
    rw = 0;
    en = 1;
    __delay_ms(2);
    en = 0;
}

void show(const char *s)
{
    while(*s)
        dat(*s++);
    
}

void main()
{
    TRISD = 0X00;
    TRISB0 = 0;
    TRISB1 = 0;
    TRISB2 = 0;

    lcd_init();

    show("Time:");
    cmd(0xC0);
    show("Date:");

    rtc_init();
    
    while(1)
    {
        sec   = rtc_read_byte(0x00)& 0X7F;
        min   = rtc_read_byte(0x01);
        hour  = rtc_read_byte(0x02);
        day   = rtc_read_byte(0x03);
        date  = rtc_read_byte(0x04);
        month = rtc_read_byte(0x05);
        year  = rtc_read_byte(0x06);

        
        cmd(0x85);
        dat(convup(hour));
        dat(convd(hour));
        dat(':');
        dat(convup(min));
        dat(convd(min));
        dat(':');
        dat(convup(sec));
        dat(convd(sec));

        
        cmd(0xC5);
        dat(convup(date));
        dat(convd(date));
        dat('/');
        dat(convup(month));
        dat(convd(month));
        dat('/');
        dat(convup(year));
        dat(convd(year));

        __delay_ms(500);
    }
}

void rtc_init()
{
    TRISC3 = 1;
    TRISC4 = 1;
    SSPCON = 0x28;
    SSPADD = 9;
}

void waitmssp()
{
    while(!SSPIF);
    SSPIF = 0;
}

void rtc_send(unsigned char a)
{
    SSPBUF = a;
    waitmssp();
    while(ACKSTAT);
}

void rtc_send_byte(unsigned char addr, unsigned char data)
{
    rtc_start();
    rtc_send(0xD0);
    rtc_send(addr);
    rtc_send(data);
    rtc_stop();
}

unsigned char rtc_read()
{
    RCEN = 1;
    waitmssp();
    return SSPBUF;
}

unsigned char rtc_read_byte(unsigned char addr)
{
    unsigned char rec;

L:  rtc_res();
    SSPBUF = 0xD0;
    waitmssp();
    while(ACKSTAT){goto L;}
    rtc_send(addr);
    rtc_res();
    rtc_send(0xD1);
    rec = rtc_read();
    rtc_nak();
    rtc_stop();

    return rec;
}

void rtc_start()
{
    SEN = 1;
    waitmssp();
}

void rtc_stop()
{
    PEN = 1;
    waitmssp();
}

void rtc_res()
{
    RSEN = 1;
    waitmssp();
}

void rtc_ack()
{
    ACKDT = 0;
    ACKEN = 1;
    waitmssp();
}
void rtc_nak()
{
    ACKDT = 1;
    ACKEN = 1;
    waitmssp();
}

unsigned char convup(unsigned char bcd)
{
    return ((bcd >> 4) + 48);
}

unsigned char convd(unsigned char bcd)
{
    return (bcd & 0x0F) + 48;
}

