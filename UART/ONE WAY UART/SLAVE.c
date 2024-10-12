#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 6000000      // Define the external crystal free
unsigned char bs;
void init();
void main(void) {
    init();
   
    while(1)
    {
        if(PIR1 & 0x20)
        {
        bs = RCREG;
        }
        switch(bs)
        {
            case 'A':
                PORTD = 0x10;
                break;
            case 'B':
                PORTD = 0x02;
                break;
            case 'C':
                PORTD = 0x12;
                break;
            case 'D':
                PORTD = 0x00;
                break;
            default:
                 PORTD = 0x00;
                 break;
        }
                
                
                
        
    }
        
    
    return;
}
void init()
{
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISC = 0xC0;
    TXSTA = 0x20;
    RCSTA = 0x90;
    SPBRG = 0x09;
}
