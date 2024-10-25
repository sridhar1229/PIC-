#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 6000000      // Define the external crystal frequency as 6 MHz
unsigned char C = 0;            // Overflow counter

void main(void) {
    // Initialize Ports
    TRISB = 0x00;
    PORTB = 0x00;
    TRISC = 0x00;
    PORTC = 0x00;
    
    // Initialize Timer1
    TMR1 = 5336;   // Preload the calculated value for fractional overflow
    T1CON = 0x01;  // Enable Timer1 with a 1:1 prescaler
    
    // Enable Interrupts
    INTCON |= 0xC0;   // Enable GIE and PEIE (Global and Peripheral Interrupts)
    PIE1 |= 0x01;     // Enable Timer1 Interrupt
    PIR1 &= 0xFE;     // Clear the Timer1 Interrupt flag
    
    // Set PORTB initially high (to indicate the LED state)
    PORTB = 0xFF;
    
    while(1)
    {
        // Main loop toggles PORTC every 500 ms (for testing purposes)
        PORTC = ~PORTC;
        __delay_ms(500);
    }
}

void __interrupt() ISR()
{
    // Check if Timer1 has overflowed
    if (PIR1 & 0x01)
    {
        C++;  // Increment the overflow counter
        if (C == 25)  // After 25 full overflows + 1 fractional overflow
        {
            // Toggle LED connected to PORTB
            PORTB = ~PORTB;
            C = 0;  // Reset the overflow counter
        }
        // Preload Timer1 for the fractional overflow
        TMR1 = 5336;
        PIR1 &= (~0x01);  // Clear Timer1 interrupt flag
    }
}
