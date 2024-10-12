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
            bs = PORTB; // read the port b
        switch(bs)
        {
            case 0xE0:// if rd4 press
                TXREG = 'A';// using this register we transmit the data
                __delay_ms(100);
                break;
            case 0xD0:// if rd5 press
                TXREG = 'B';// using this register we transmit the data
                __delay_ms(100);
                break;
            case 0xB0:// if rd6press
                TXREG = 'C';// using this register we transmit the data
                __delay_ms(100);
                break;
            case 0x70:// if rd7 press
                TXREG = 'D';// using this register we transmit the data
                __delay_ms(100);
                break;
//            default:
//                TXREG = 'D';// using this register we transmit the data
//                __delay_ms(100);
//                break;
                
        }
                 
    }
          
   
}
void init()
{
    TRISB = 0xF0;//setting the RD4 to RD7 as input
    PORTB = 0x00;// defsult low
    OPTION_REG &= 0x7F;// internal pullup
    
    TRISC = 0xC0;//1100 0000
    TXSTA = 0x20;// enable the transmmision by enable the TXEN
    RCSTA = 0x90;// enable the recevier by enable the by spen and cren
    SPBRG = 0x09;// baud rate as 9600
}
