#ifndef LCD_H
#define LCD_H

#include <xc.h>

// Define pins
#define RS LATCbits.LATC0
#define EN LATCbits.LATC1
#define D4 LATDbits.LATD4
#define D5 LATDbits.LATD5
#define D6 LATDbits.LATD6
#define D7 LATDbits.LATD7

// Function prototypes
void LCD_Init();
void LCD_Clear();
void LCD_Set_Cursor(unsigned char, unsigned char);
void LCD_Write_Char(char);
void LCD_Write_String(char *);

#endif
