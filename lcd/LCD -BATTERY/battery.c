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
unsigned char da[] = "BATT VOLT:", d1[] = "LOW   ", d2[] = "NORMAL    ", d3[] = "HIGH     ", d4[] = "BATTERY";

void init();
void command(unsigned char);
void data(unsigned char);
void convert(float);
void split(int);
void check(float a);

void check(float a){// CHECK TO PRINT THE VOLTAGE HIGH OR LOW
if (a <= 17.5) {// IF LESS THAN 17.5 THEN LOW
        command(0xC8);// TO PRINT THE 2 ROW 8 COLUMN
        for (int i = 0; d1[i] != '\0'; i++) {// READ UNTILL THE THE NULL MENAS END
            data(d1[i]);
        }
    } else if (a > 17.5 && a < 20.5) {    
        command(0xC8);
        for (int i = 0; d2[i] != '\0'; i++) {//READ UNTILL THE THE NULL MENAS END
            data(d2[i]);
        }
    } else {
        command(0xC8);
        for (int i = 0; d3[i] != '\0'; i++) {//READ UNTILL THE THE NULL MENAS END
            data(d3[i]);
        }
    }
}


void convert(float a) {// CONVERT FLOAT INTO INT
    int s = (int)(a * 10);// WHEN MULTIPLY BY 10 THE DECIMAL POINT OF SINGLE DIGIT FLOAT WILL BE 0
    split(s);// CALL THE SPLIT FUN
}

unsigned char x;
float a = 17.5;// 

void main() {
    init();
    TRISB = 0xF0; // Set RB0-RB3 as outputs, RB4-RB7 as inputs
    PORTB = 0x00; // portb as low
    OPTION_REG &= 0x7F; // set the special register to pull up
    
    init(); // call the initialization function
    
    while (1) {
        int val = PORTB; // store the portb value in val
        switch(val) {
            case 0xD0: // rc5 is pressed 1101 000
                    a = (float)(a + 0.1); // Increment the value
                command(0x8B);// 
                if (a >= 22.5) {// CHENCK GRATER THAN 22.5
                    a = 22.5; // TRUE SO SET IT AS 22,5
                    convert(a);// CALL CONVERT TO DISPLAY THE VALUE IN THE LCD
                    check(a);// CALL CHECK FUNCTION
                } else {
                    convert(a);//CALL CONVERT TO DISPLAY THE VALUE IN THE LCD
                    __delay_ms(2);
                     check(a);// CALL CHECK FUNCTION
                }
                break;
                
            case 0xB0: // rc is pressed 1011 0000
               a = (float)(a - 0.1);//DECREMENT THE VALUE
                command(0x8B);//SET THE CURSOR IN 1ST ROW 11 COLUMN
                if (a <= 15.5) {// CHECK LESS THAN 15.5
                    a = 15.5;// TRUE MEANS SET 15.5 AS DEFAULT
                    convert(a);// CALL THE FUNC
                     check(a);// CALL THE FUNCTION
                } else {
                    convert(a);// CALL THE FUNCTION
                    __delay_ms(2);
                     check(a);// CALL THE FUNCTION
                }
                break;
                
            case 0x70: // rc7 is pressed 0111 0000
                command(0x8B);//SET THE CURSOR IN 1ST ROW 11 COLUMN
                a = 17.5;//SET A AS 17.5
                convert(a);// CALL THE FUNCTION
                 check(a);// CALL THE FUNCTION
                break;
                
            case 0xE0: // rc4 is pressed 1110 0000
                for (x = 0; x < 17; x++) {
                    command(0x80 + x); // increment the cursor position instead of 0x06
                    data(da[x]); // read the data
                }
                command(0x8B);
                split(175);// CALL THE SPLIT WITH INT TYPE FOR 17.5
                for (x=0;x < 7;x++)
                {
                command(0xC0 + x);// INCREMENT THE CURSOR
                    data(d4[x]);//READ THE DATA
                    
                }
                check(a);// CALL THE FUNCTION
               
                
                break;
        }
        
        
        
}
}

void split(int m) {
    
    int d = m / 100;         // Get the integer part
    int e = (m % 100) / 10;  // Get the first decimal place
    int f = m % 10;          // Get the second decimal place

    data((unsigned char)(0x30 + d)); //PRINT THE FIRST DIGIT TO SEND IN HEXADECIMAL;
    data((unsigned char)(0x30 + e))//PRINT THE SECOND DIGIT TO SEND IN HEXADECIMAL;
    data(0x2E);// PRINT THE DOT SEND AS HEXADECIMAL
    data((unsigned char)(0x30 + f));//PRINT THE DECIMAL DIGIT TO SEND IN HEXADECIMAL;
}

void init() {
    TRISC = 0x00; // OUTPUT MODE SET
    TRISD = 0x00; // SET OUTPUT MODE
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
    __delay_ms(100);
}

void data(unsigned char i) {
    PORTC |= 0x08;   // RS = 1 for data (RC3 is HIGH) 0000 1000
    PORTD = i;       // Send data to PORTD
    PORTC |= 0x01;   // Enable = 1 (RC0 is HIGH)
    __delay_ms(10);   
    PORTC &= ~0x01;  // Enable = 0 (RC0 is LOW)
    __delay_ms(10);  
}

void command(unsigned char i) {
    PORTC &= ~0x08;  // RS = 0 for command (RC3 is LOW)
    PORTD = i;       // Send command to PORTD
    PORTC |= 0x01;   // Enable = 1 (RC0 is HIGH)
    __delay_ms(10);   
    PORTC &= ~0x01;  // Enable = 0 (RC0 is LOW)
    __delay_ms(10);  
}
