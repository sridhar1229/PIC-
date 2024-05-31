/*
 * File:   multiplexing_7-segment.c
 * Author: thami
 *
 * Created on 31 May, 2024, 4:49 PM
 */


#include <xc.h>
#define _XTAL_FREQ 20000000
#define ON 0
#define OFF 1

unsigned char n =69;
unsigned char i,j;
unsigned int s[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
void main(void) 
{
    
    TRISB = 0x00;
    TRISC = 0x00;
    
    while(1)
    {
        i=n/10;
        j=n%10;
        PORTCbits.RC0 =ON ;
        PORTCbits.RC1=OFF;
        PORTB =s[i];
       __delay_ms(10);
        PORTCbits.RC1=ON ;
        PORTCbits.RC0=OFF;
        PORTB =s[j];
        __delay_ms(10);
    }
        
        
    
}
