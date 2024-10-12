#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#include <xc.h>
#define _XTAL_FREQ 6000000

void main(void) {
    TRISC = 0x00; // Set PORTC as output
    TRISB = 0x01; // Set RB0 as input (bitwise)
    int c = 0;
    PORTB= 0x00;
    unsigned int s[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

    PORTC = s[0];
    while(1){
        if(PORTB == 0x01){
            __delay_ms(50); // Simple debounce delay
            if(PORTB == 0x01){ // Check if button is still pressed
                c = c + 1;
                if (c >= 10) { // Reset counter after 9
                    c = 0;
                }
                while(PORTB == 0x01); // Wait until the button is released
            }
        }
        
        PORTC = (~s[c]); // Display the current counter value on 7-segment display
        __delay_ms(100); // Update display every 100ms
    }
}
