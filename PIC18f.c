#include <xc.h>
#include <stdio.h>
#include "lcd.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ 20000000     // Define the system clock

#define DHT11_PIN RA0           // DHT11 data pin
#define PIR_PIN RA1             // PIR sensor pin
#define BUTTON1 RB0             // Button 1 pin
#define BUTTON2 RB1             // Button 2 pin

void LCD_Init();
void DHT11_Init();
void DHT11_Read(int *temperature, int *humidity);
unsigned char DHT11_Check();
unsigned char DHT11_ReadByte();

void main(void)
{
    int temperature = 0, humidity = 0;
    int students = 0;
    char buffer[16];
    int option = 0; // 0: Temperature & Humidity, 1: Students detected

    TRISA = 0xFF;  // Set PORTA as input (for DHT11 and PIR)
    TRISB = 0xFF;  // Set PORTB as input (for Buttons)
    TRISC = 0x00;  // Set PORTC as output (for LCD control)
    TRISD = 0x00;  // Set PORTD as output (for LCD data)

    LCD_Init();    // Initialize the LCD

    while(1)
    {
        // Check Button 1
        if(BUTTON1 == 0)
        {
            option = 0; // Temperature & Humidity
            __delay_ms(100); // Debouncing delay
        }
        // Check Button 2
        if(BUTTON2 == 0)
        {
            option = 1; // Students detected
            __delay_ms(100); // Debouncing delay
        }

        if(option == 0) // Temperature & Humidity
        {
            DHT11_Read(&temperature, &humidity);  // Read DHT11 values
            sprintf(buffer, "Temp: %02dC", temperature);
            LCD_Clear();
            LCD_Set_Cursor(1, 1);
            LCD_Write_String(buffer);
            sprintf(buffer, "Hum: %02d%%", humidity);
            LCD_Set_Cursor(2, 1);
            LCD_Write_String(buffer);
        }
        else if(option == 1) // Students detected
        {
            if(PORTAbits.RA1) // Check PIR sensor
            {
                students++;
                __delay_ms(1000); 
            }
            LCD_Clear();
            LCD_Set_Cursor(1, 1);
            sprintf(buffer, "Students: %d", students);
            LCD_Write_String(buffer);
        }
    }
}

void DHT11_Init()
{
    TRISA0 = 0; // Set RA0 as output
    LATAbits.LATA0 = 0;
    __delay_ms(18); // Send start signal
    LATAbits.LATA0 = 1;
    __delay_us(20);
    TRISA0 = 1; // Set RA0 as input
}

unsigned char DHT11_Check()
{
    unsigned char response = 0;
    __delay_us(40);
    if (!PORTAbits.RA0)
    {
        __delay_us(80);
        if (PORTAbits.RA0) response = 1;
        __delay_us(40);
    }
    return response;
}

unsigned char DHT11_ReadByte()
{
    unsigned char i, byte = 0;
    for (i = 0; i < 8; i++)
    {
        while (!PORTAbits.RA0); // Wait for pin to go high
        __delay_us(30);
        if (PORTAbits.RA0) byte |= (1 << (7 - i)); // If high after 30 us, it is a '1'
        while (PORTAbits.RA0); // Wait for pin to go low
    }
    return byte;
}

void DHT11_Read(int *temperature, int *humidity)
{
    unsigned char temp_int, temp_dec, hum_int, hum_dec, checksum;

    DHT11_Init();
    if (DHT11_Check())
    {
        hum_int = DHT11_ReadByte();
        hum_dec = DHT11_ReadByte();
        temp_int = DHT11_ReadByte();
        temp_dec = DHT11_ReadByte();
        checksum = DHT11_ReadByte();

        if ((hum_int + hum_dec + temp_int + temp_dec) == checksum)
        {
            *temperature = temp_int;
            *humidity = hum_int;
        }
    }
}
