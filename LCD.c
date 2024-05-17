#include "lcd.h"
#define _XTAL_FREQ 20000000

void LCD_Enable()
{
    EN = 1;
    __delay_us(10);
    EN = 0;
    __delay_us(100);
}

void LCD_Cmd(unsigned char cmd)
{
    RS = 0;
    D4 = (cmd >> 4) & 0x01;
    D5 = (cmd >> 5) & 0x01;
    D6 = (cmd >> 6) & 0x01;
    D7 = (cmd >> 7) & 0x01;
    LCD_Enable();
    D4 = cmd & 0x01;
    D5 = (cmd >> 1) & 0x01;
    D6 = (cmd >> 2) & 0x01;
    D7 = (cmd >> 3) & 0x01;
    LCD_Enable();
}

void LCD_Write_Char(char data)
{
    RS = 1;
    D4 = (data >> 4) & 0x01;
    D5 = (data >> 5) & 0x01;
    D6 = (data >> 6) & 0x01;
    D7 = (data >> 7) & 0x01;
    LCD_Enable();
    D4 = data & 0x01;
    D5 = (data >> 1) & 0x01;
    D6 = (data >> 2) & 0x01;
    D7 = (data >> 3) & 0x01;
    LCD_Enable();
}

void LCD_Init()
{
    TRISC0 = 0; // RS pin 
    TRISC1 = 0; // EN pin 
    TRISD4 = 0; // Data pins output
    TRISD5 = 0;
    TRISD6 = 0;
    TRISD7 = 0;
    __delay_ms(20);
    LCD_Cmd(0x02);  // 4-bit mode
    LCD_Cmd(0x28);  // 2 line, 5x7 matrix
    LCD_Cmd(0x0C);  // Display on, cursor off
    LCD_Cmd(0x06);  // Increment cursor
    LCD_Cmd(0x01);  // Clear display
    __delay_ms(2);
}

void LCD_Clear()
{
    LCD_Cmd(0x01);  // Clear display
    __delay_ms(2);
}

void LCD_Set_Cursor(unsigned char row, unsigned char column)
{
    unsigned char temp;
    if(row == 1)
        temp = 0x80 + column - 1;
    else if(row == 2)
        temp = 0xC0 + column - 1;
    LCD_Cmd(temp);
}

void LCD_Write_String(char *str)
{
    while(*str)
        LCD_Write_Char(*str++);
}
