#include <pic18f4550.h>  
#include "lcd.h"
#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 20000000     // Define the system clock
#define DHT11_PIN RA0 // DHT11 data pin
#define PIR_PIN   RA1   // PIR sensor pin
#define BUTTON1   RB0   // Button 1 pin
#define BUTTON2   RB1// Button 2 pin

void DHT11_Start();
unsigned char DHT11_CheckResponse();
unsigned char DHT11_ReadData();

void main(void)
{
    int temperature = 0, humidity = 0;
    int students = 0;
    char buffer[16];
    int pir_last_state = 0;

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
            DHT11_Start();
            if(DHT11_CheckResponse())
            {
                unsigned char data[5];
                int i;
                
                for(i = 0; i < 5; i++) {
                    data[i] = DHT11_ReadData();
                }
                
                if(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
                    humidity = data[0];
                    temperature = data[2];
                }
            }
            LCD_Clear();
            LCD_Set_Cursor(1, 1);
            sprintf(buffer, "Temp: %02dC", temperature);
            LCD_Write_String(buffer);
            LCD_Set_Cursor(2, 1);
            sprintf(buffer, "Hum: %02d%%", humidity);
            LCD_Write_String(buffer);
            __delay_ms(200); // Debouncing delay
        }

        // Check Button 2
        if(BUTTON2 == 0)
        {
            if(PIR_PIN && !pir_last_state) // Check PIR sensor
            {
                students++;
                __delay_ms(1000); // Delay to count only once
            }
            pir_last_state = PIR_PIN; // Update last PIR state
            LCD_Clear();
            LCD_Set_Cursor(1, 1);
            sprintf(buffer, "Students: %d", students);
            LCD_Write_String(buffer);
            __delay_ms(300); // Debouncing delay
        }

        __delay_ms(200); // Delay to prevent LCD flickering
    }
}

void DHT11_Start()
{
    TRISA0 = 0; // Set RA0 as output
    LATAbits.LATA0 = 0; // Pull RA0 low
    __delay_ms(18); // Wait for 18ms
    LATAbits.LATA0 = 1; // Pull RA0 high
    __delay_us(20); // Wait for 20-40us
    TRISA0 = 1; // Set RA0 as input
}

unsigned char DHT11_CheckResponse()
{
    __delay_us(40);
    if (!DHT11_PIN)
    {
        __delay_us(80);
        if (DHT11_PIN)
        {
            __delay_us(40);
            return 1;
        }
    }
    return 0;
}

unsigned char DHT11_ReadData()
{
    unsigned char i, result = 0;
    for (i = 0; i < 8; i++)
    {
        while (!DHT11_PIN); // Wait for pin to go high
        __delay_us(30);
        if (DHT11_PIN) result |= (1 << (7 - i));
        while (DHT11_PIN); // Wait for pin to go low
    }
    return result;
}
