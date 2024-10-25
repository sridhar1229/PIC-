#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 20000000

unsigned char  timer_count = 0;

void interrupt timer_0()
{
    if(INTCONbits.TMR0IF == 1)
    {
        timer_count++;
        INTCONbits.TMR0IF = 0;
    }
}

        
void main(void) {
    
    TRISB = 0x00;
    PORTB =0x00;
    INTCONbits.GIE =1;
    INTCONbits.INTE =1;
    INTCONbits.TMR0IE =1;
    
    OPTION_REG = 0x07;
    TMR0 = 59;
    
    while(1)
    {
        if(timer_count == 0)
            PORTB =0xFF;
        if(timer_count == 100)
            PORTB =0x00;
        if(timer_count == 200)
            timer_count=0;
    }
   
    return;
}
