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
#include "EEPROM.h"

void main(void) {
    master_init(100000);
    unsigned int add = 0x0020;//
    unsigned char data = 0x04;
    
    my_eeprom_write(add++,data++);//increment the data
     my_eeprom_write(add++,data++);//increment the data
     my_eeprom_write(add,data);//increment the data
    __delay_ms(20);
    
    add = 0x0020;//reset the data
    TRISD = 0x00;
    
   read(add++);//read the data
    __delay_ms(1000);
    read(add++);////read the data
    __delay_ms(1000);
    read(add);////read the data
    
    while(1);

    
    return;
}
