#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#define _XTAL_FREQ 6000000
unsigned char v;

void main(void) {
    TRISB =0x0F;
    TRISC = 0x00;
    TRISD = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD= 0x00;
    OPTION_REG = 0x7F;
    INTCON = 0x90;// enable global interupt and external interupt enabler(INTE))
    while(1)
    {
      
        PORTC = ~PORTC;
        __delay_ms(500);
    }
    return;
}

void __interrupt() ISR(void)
{
      
    v =PORTB;
    if(INTCON & 0x02) // check for RBF (ecternal interupt flag)(INTF)
    {
        
    if(v = 0x0E)
    {
       PORTD = ~PORTD; 
    }
    }
    INTCON &= (~0x02);  //clear the flag  
}
