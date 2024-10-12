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
void split(unsigned int a){
    
    
     unsigned int d=a/1000;//divide by 1000 to find the thousands position
    unsigned int e=(a%1000)/100;//module divide by thousand and divide bu 100 to find the hundred position
    unsigned int g= (a%100)/10;//modulo dive by 100 so the reminder in 2 digits and dive by 10 to find the tens
    unsigned int f=a%10;// modulo divide by 10 to find the ones
       data(0x30+d);  //send data in hexadecimal
       data(0x30+e);  //send data in hexadecimal
       data(0x30+g); //send data in hexadecimal
       data(0x30+f);   //send data in hexadecimal
 
}

unsigned char x;
unsigned char a[] = "SET SPD:    rpm";
unsigned int c = 0000;

void main() {
    TRISB = 0xF0; // Set RB0-RB3 as outputs, RB4-RB7 as inputs
    PORTB=0x00; // portb as low
    OPTION_REG &= 0x7F; // set the special registe to pull up
    
    init(); //call the initilization funcation
    
    
    while (1) {
        int val =PORTB;// store the portb value in val
        switch(val)
        {
            case 0xD0:// rc5is pressed 1101 000
            __delay_ms(50); // Debounce delay
            if (PORTB==0xD0) { // Check again after delay
                c=c+1;// increament the c
            }
                // Display the updated counter value
                command(0x88);
                if(c>=5000) // if greater than 5000 
                {
                    c=5000 // set c in 5000
                    split(c); // send data c
                }
                else
                {
               split(c); //send data c
                __delay_ms(200);
                }
                break;
          case 0xB0:// rcis pressed 1011
            __delay_ms(50); // delay command
            if (PORTB==0xB0) { // Check again after delay
                c=c-1;// decreament the c
            }
                // Display the updated counter value
                command(0x88); //set the position of cursor
                // in c start with o means octal so i include greater than 5000 
                if(c == 0000|| c>=5000) // check wheather it is 0000 or greater than 5000 
                {
                    c=0000;// set c as 0000
                    split(c);//
                }
                else
                    split(c);
                __delay_ms(200);
                
                break;
        case 0x70:// rc7is pressed 0111 0000
            command(0x88);
            split(0000); //set the data as 0000
            break;
        case 0xE0:// rc4is pressed 1110 0000
            for (x = 0; x < 17; x++) {
        command(0x80+x); //increment the cursor position instead of 0x06
        data(a[x]);// read the data
    }
            
        }
        
            }
        
    }    

void init() {
    TRISC = 0xF0; // Set RC0-RC3 as outputs for LCD control, RC4-RC7 as inputs
    TRISD = 0x00; // Set PORTD as output for LCD data
    
    // LCD initialization sequence
    command(0x38); // Function set: 8-bit, 2 line, 5x7 dots
    __delay_ms(100);
    command(0x0C); // Display on, cursor off
    __delay_ms(100);
    command(0x01); // Clear display
    __delay_ms(100);
}

void data(unsigned char i) {
    PORTC |= 0x08;   // RS = 1 for data
    PORTD = i;       // Send data to PORTD
    PORTC |= 0x01;   // Enable = 1
    __delay_ms(2);   // Short delay for data to latch
    PORTC &= ~0x01;  // Enable = 0
    __delay_ms(2);   // Wait for the data to be processed
}

void command(unsigned char i) {
    PORTC &= ~0x08;  // RS = 0 for command
    PORTD = i;       // Send command to PORTD
    PORTC |= 0x01;   // Enable = 1
    __delay_ms(2);   // Short delay for command to latch
    PORTC &= ~0x01;  // Enable = 0
    __delay_ms(2);   // Wait for the command to be processed
}


