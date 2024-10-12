#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 6000000      // Define the external crystal free
void command(unsigned char);
void data(unsigned char);
void init_lcd(void);
unsigned char a[13] = {"RECEIVE DATA:"};
unsigned char b[16] = {"TRANSMIT DATA:"};
unsigned char r;

void init()
{
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISB = 0x00; //OUTPUT MODE SET
    PORTB = 0x00;
   TRISC = 0xC0;
   TXSTA = 0x20;
   RCSTA = 0x90;
   SPBRG = 0x09;
   
}
void main(void) {
   // TRISA =0x00;
    
   // __delay_ms(1000);
    init();
   // __delay_ms(1000);
    init_lcd();
    
     for (int x = 0; x < 13; x++) {
        data(a[x]);  //READ DATA INDEX BY INDEX
    }
    command(0xC0);
    for (int x = 0; x < 14; x++) {
        data(b[x]);  //READ DATA INDEX BY INDEX
    }
        
    while(1)
    {
        if(PIR1 & 0x20)
        {
//            if (RCSTA & 0x02) {
//               RCSTA = RCSTA & 0xEF;  // Clear the CREN bit (disable reception)
//               __delay_ms(500);
//               RCSTA = RCSTA | 0x10;  // Set the CREN bit (re-enable reception)
//           }
            r = RCREG;
            command(0x8E);
            data(r);
            
        }

            if(r == 'A')
            {
              TXREG = 'a';
              command(0xCE);
              data(r+32);
              
             __delay_ms(1000);

            }

            else if(r == 'B')
            {

                 TXREG = 'b';
                 command(0xCE);
                 data(r+32);
                __delay_ms(1000);
                    
            }
            else if(r== 'C')
            {
            
                TXREG = 'c';
               // __delay_ms(3000);
                command(0xCE);
                 data(r+32);
                 __delay_ms(1000);

            }
            else if(r == 'D')
            {

              TXREG ='d'; 
              command(0xCE);
              data(r+32);
             __delay_ms(1000);
              
            }

        }
}

void init_lcd()
{
   
    command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(10);
    command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(10);
        command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(10);
    command(0x38);  // TO MAKE THE LCD DOT 8*5 ELEMENTS SET OR ON
    __delay_ms(10);
    command(0x0C);  // Display ON, cursor OFF
    __delay_ms(10);
    command(0x06);  // SET TO TO THE LEFT TO RIGHT NORMAL POSITION
    __delay_ms(10);
    command(0x01);  // Clear display
    __delay_ms(10);
    
}

void command(unsigned char i) 
{
    PORTB &= ~0x08;  // RS = 0 for command (RC3 is LOW)
    PORTD = i;       // Send command to PORTD
    PORTB |= 0x01;   // Enable = 1 (RC0 is HIGH)
    __delay_ms(10);   
    PORTB &= ~0x01;  // Enable = 0 (RC0 is HIGH)
    __delay_ms(10);  
}
void data(unsigned char i)
{
    //OR OPERATION IS USED TO SET THE BITS 
    // AND OPERATION IS USED TO CLEAR THE BITS USED WITH NOT OPERATOR
    PORTB |= 0x08;   // RS = 1 for data (RC3 is HIGH) 0000 1000
    PORTD = i;       // Send data to PORTD
    PORTB |= 0x01;   // Enable = 1 (RC0 is HIFH)
    __delay_ms(10);   
    PORTB &= ~0x01;  // Enable = 0 (RC0 is LOW)
    __delay_ms(10);  
}
