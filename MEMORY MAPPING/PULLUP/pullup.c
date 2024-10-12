#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include "pullup_function.h"


void main(void) {
    init_button();//TRISB IN OUTPUT
    init_internalpullup();// OPTTIONAL REGISTOR ENABLE
    init_led();// TRISD AS OUTPUT
    while(1)
    {
           
        switch(button())// READING BUTTON STATE
        {
            case 0xFE: //1111 1110
                led_on(1);// PATTERN 1
                break;
            case 0xFD: //1111 1101
                led_on(2);//PAATERN 2
                break;
            case 0xFB://1111 1011
                led_on(3);//PATTERN 3
                break;
            case 0xF7://1111 0111
                led_on(0);//PATTERN 4
                break;
        }
    }
  
}
