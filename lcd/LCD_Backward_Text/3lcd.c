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

void init();
void command(unsigned char);
void data(unsigned char);

unsigned char x, i ;
unsigned char a[11] = {"HELLO WORLD"};

void main() {
    init();
    command(0x80);   // SET THE CURSOR AT THE FIRST COLUMNS AT FIRST ELEMEMNT
    for (x = 0; x < 11; x++) {
        data(a[x]);  //READ DATA INDEX BY INDEX
        command(0x80+x);
    }
    while (1);
}

void init() {
    TRISC = 0x00;// SET IN OUTPUT MODE
    TRISD = 0x00;//  SET IN OUTPUT MODE
    command(0x38);  // 
    __delay_ms(100);
    command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(100);
    command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(100);
    
    command(0x0C);  // Entry mode set
    __delay_ms(100);
    command(0x01);  // Clear display
    __delay_ms(100);
}

void data(unsigned char i) {
      //OR OPERATION IS USED TO SET THE BITS 
    // AND OPERATION IS USED TO CLEAR THE BITS USED WITH NOT OPERATOR
    PORTC |= 0x08;   // RS = 1 for data (RC3 is ON)
    PORTD = i;       // Send data to PORTD
    PORTC |= 0x01;   // Enable = 1 (RC0 is ON)
    __delay_ms(100);  
    PORTC &= ~0x01;  // Enable = 0 (RC0 is LOW
    __delay_ms(100);   
}

void command(unsigned char i) {
    PORTC &= ~0x08;  // RS = 0 for command (RC3 is cleared)
    PORTD = i;       // Send command to PORTD
    PORTC |= 0x01;   // Enable = 1 (RC0 is ON)
    __delay_ms(100);   
    PORTC &= ~0x01;  // Enable = 0 (RC0 is LOW)
    __delay_ms(100);   
}
