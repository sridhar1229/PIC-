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
void pwm();

unsigned char pwmlow1 = 0x09, pwmhighl = 0x20;  // 20% duty cycle
unsigned char pwmlow2 = 0x1C, pwmhigh2 = 0x10;  // 60% duty cycle
unsigned char pwmlow3 = 0x2A, pwmhigh3 = 0x10;  // 90% duty cycle

void main(void) 
{
    init();
    while (1)
    {
        pwm();
    }
}

void init()
{
    TRISC = 0xFD;    // Set RC1 as output (0), others as input (1)
    CCP2CON = 0x0C;  // Set CCP2 to PWM mode
    T2CON = 0x06;    // Timer2 on, prescaler 1:16
    PR2 = 0x2E;      // 0010 1110 46
}

void pwm()
{
    CCPR2L = pwmlow1;           //directly set low pin
    CCP2CON = (CCP2CON & 0xCF) | pwmhighl; // to set in the 5:4 bit and with mask of ccp2con and or withhigh
    __delay_ms(3000);            // 3 seconds delay

    CCPR2L = pwmlow2;            //directly set low pin
    CCP2CON = (CCP2CON & 0xCF) | pwmhigh2; // to set in the 5:4 bit and with mask of ccp2con and or withhigh
    __delay_ms(3000);            // 3 seconds delay

    CCPR2L = pwmlow3;            // directly set low pin
    CCP2CON = (CCP2CON & 0xCF) | pwmhigh3; //to set in the 5:4 bit and with mask of ccp2con and or withhigh
    __delay_ms(3000);            // 3 seconds delay
}
