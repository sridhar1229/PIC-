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
unsigned char bs,receive;
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
                __delay_ms(1000);
                break;
            case 0xD0:// if rd5 press
                TXREG = 'B';// using this register we transmit the data
               __delay_ms(1000);
                break;
            case 0xB0:// if rd6 press
                TXREG = 'C';// using this register we transmit the data
                __delay_ms(1000);
                break;
            case 0x70:// if rd7 press
                TXREG = 'D';// using this register we transmit the data
                __delay_ms(1000);
                break;
          
                
        }
        
        if(PIR1 & 0x20)
        {   
        receive = RCREG;
        }
       
        switch(receive)
        {
            case 'a':
                PORTD =0x10;
                break;
            case 'b':
               PORTD =0x02;
                break;
            case 'c':
                 PORTD =0x12;
                break;
            case 'd':
                 PORTD =0x00;
                break;
            default:
                PORTD =0x00;
                 break;
        } 
        }
    }

void init()
{
    TRISB = 0xF0;//setting the RD4 to RD7 as input
    TRISD = 0x00;//setting the RD4 to RD7 as input
    PORTB = 0x00;// defsult low
    OPTION_REG &= 0x7F;// internal pullup
    
    PORTD = 0X00;
    TRISC = 0xC0;//1100 0000
    PORTC =0X00;
    TXSTA = 0x20;// enable the transmmision by enable the TXEN
    RCSTA = 0x90;// enable the recevier by enable the by spen and cren
    SPBRG = 0x09;// baud rate as 9600
}


