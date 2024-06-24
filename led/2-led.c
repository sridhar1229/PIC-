// CONFIG
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

void main(void) {
    TRISD = 0x00; //0000 0000 make all bits as output
    while(1){
     
    PORTD =0x40;//0100 0000 R6-ON R1 -OFF
   __delay_ms(3000);
   PORTD =0x00;//0000 0000 R6-OFF R1 - OFF
   __delay_ms(3000);
   PORTD =0x02;//0000 0010 R6-OFF R1 -ON
   __delay_ms(3000);
   PORTD =0x42;//0100 0010  R6-ON R1 -ON
   __delay_ms(3000);
    }
}

    
