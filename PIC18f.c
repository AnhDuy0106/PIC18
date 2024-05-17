#include <xc.h>
#include <stdio.h>
#include "lcd.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ 20000000     // Define the system clock

void ADC_Init()
{
    ADCON1 = 0x0E;  // Set AN0 as analog and the rest as digital
    ADCON2 = 0xA9;  // Right justify result, acquisition time 4 TAD, FOSC/8
    ADCON0 = 0x01;  // Enable ADC and select AN0
}

unsigned int ADC_Read(unsigned char channel)
{
    ADCON0 &= 0xC7;            // Clear the channel selection bits
    ADCON0 |= (channel << 3);  // Select the required channel
    __delay_ms(2);             // Acquisition time to charge the hold capacitor
    ADCON0bits.GO = 1;         // Start conversion
    while(ADCON0bits.GO);      // Wait for the conversion to finish
    return ((ADRESH << 8) + ADRESL); // Return the 10-bit ADC result
}

void main(void)
{
    unsigned int adc_value;
    char buffer[16];

    TRISA = 0xFF;  // Set PORTA as input (for ADC)
    TRISC = 0x00;  // Set PORTC as output (for LCD control)
    TRISD = 0x00;  // Set PORTD as output (for LCD data)

    ADC_Init();    // Initialize the ADC
    LCD_Init();    // Initialize the LCD

    while(1)
    {
        adc_value = ADC_Read(0);  // Read the ADC value from channel 0
        sprintf(buffer, "ADC: %04u", adc_value);  // Convert ADC value to string
        LCD_Clear();
        LCD_Set_Cursor(1, 1);
        LCD_Write_String(buffer);  // Display the ADC value on the LCD
        __delay_ms(500);           // Delay to make the reading visible
    }
}
