#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#define _XTAL_FREQ 6000000      // Define the external crystal frequency
#define CALIBRATION_VALUE 338    // Calibration value for voltage conversion
#define BITS 1023               // Maximum ADC value (10-bit ADC)


// Display strings
unsigned char str1[] = "ADC RAW:      ";
unsigned char str2[] = "ADC CAL:";

unsigned  long int result1;            // Variable to store ADC result
unsigned int volt;              // Variable to store calculated voltage
unsigned int adchigh1, adclow1;   // Variables to store high and low bytes of ADC result


void init();                    // Function to initialize LCD and ADC
void command(unsigned char);    // Function to send a command to the LCD
void data(unsigned int);       // Function to send data to the LCD
void split(unsigned int a);     // Function to split and display the ADC result
void voltage(unsigned int s);   // Function to display the calculated voltage

unsigned long int adc(int k)
{
    if (k == 0) 
    {
       ADCON0 = 0x81;          // Configure ADC (AN0 selected, ADC enabled)
    }
    else
    {
        ADCON0 = 0x91;       // SELECTING CHANNEL 2
    }
        __delay_ms(5);          // Wait for ADC setup
        ADCON0 |= 0x04;         // Start ADC conversion
        while(ADCON0 & 0x04);   // Wait for conversion to complete
        
        adchigh1 = ADRESH;       // Store high 8 bits of ADC result
        adclow1 = ADRESL;        // Store low 8 bits of ADC result
        result1 = (adchigh1 << 8) | adclow1; // Combine high and low bytes
        
        //unsigned int pwm_duty = (result * 255) / 1023; // Scale the ADC value to 0-255 range 
      return result1;  
}

void voltage(unsigned int s)
{
    if(s < 10)                  // Single digit voltage
    {
        data(0x30 + s);         // Send the voltage value as ASCII
        data(0x56);             // Send 'V' to the LCD
        data(0x20);             // Send space
    }
    else if(s >= 10 && s < 100) // Two-digit voltage
    {
        unsigned int h = s / 10;
        unsigned int g = s % 10;
        data(0x30 + h);         // Send tens place
        data(0x30 + g);         // Send ones place
        data(0x56);             // Send 'V' to the LCD
        data(0x20);              // Send space
    }
    else
    {
        unsigned int k = s / 100;
        unsigned int l= (s % 100) / 10;
        unsigned int m = s % 10;
        data(0x30 + k);         // Send hundreds place
        data(0x30 + l);         // Send tens place
        data(0x30 + m);         // Send ones place
        data(0x56);             // Send 'V' to the LCD
        data(0x20);              // Send space     
    }
}

// Function to display the raw ADC result on the LCD
void split(unsigned int a) 
{
    if(a < 10)                  // Single digit ADC value
    {
        data(0x30 + a);         // Send the ADC value as ASCII
        data(0x20);             // Send space
        data(0x20);             // Send space
        data(0x20);             // Send space
    }
    else if(a >= 10 && a < 100) // Two-digit ADC value
    {
        unsigned int b = a / 10;
        unsigned int c = a % 10;
        data(0x30 + b);         // Send tens place
        data(0x30 + c);         // Send ones place
        data(0x20);             // Send space
        data(0x20);             // Send space
    }
    else if (a >= 100 && a < 1000) // Three-digit ADC value
    {
        unsigned int b = a / 100;
        unsigned int c = (a % 100) / 10;
        unsigned int d = a % 10;
        data(0x30 + b);         // Send hundreds place
        data(0x30 + c);         // Send tens place
        data(0x30 + d);         // Send ones place
        data(0x20);             // Send space
    }
    else                       // Four-digit ADC value
    {
        unsigned int b = a / 1000;
        unsigned int d = (a % 1000) / 100;
        unsigned int e = (a % 100) / 10;
        unsigned int f = a % 10;
        data(0x30 + b);         // Send thousands place
        data(0x30 + d);         // Send hundreds place
        data(0x30 + e);         // Send tens place
        data(0x30 + f);         // Send ones place
        data(0x20);             // Send space
        data(0x20);             // Send space
    }     
}

void main(void) {
    init();                     // Initialize the LCD and ADC
    
    command(0x80);              // Move cursor to the beginning of the first line
    for (int i = 0; i < 14; i++) {
        data(str1[i]);          // Display "ADC RAW:" on the LCD
    }
    
    command(0xC0);              // Move cursor to the beginning of the second line
    for (int i = 0; i < 14; i++) {
        data(str2[i]);          // Display "ADC CAL:" on the LCD
    }
    while(1) {
        unsigned  long int adc1 = adc(0); // CALL THE ADC FUNC
        command(0x89);          // Move cursor to the correct position for raw ADC result
        split(adc1);          // Display raw ADC result on the LCD
        
        unsigned  long int adc2 = adc(1);
        volt = (adc2 * CALIBRATION_VALUE) / BITS; // Calculate voltage from ADC result
        
        CCPR1L = volt >> 2;     // Set PWM duty cycle (low 8 bits)
        CCP1CON = (CCP1CON & 0xCF) |((volt & 0x03)<< 4); // Set high 2 bits
        
        command(0xC9);          // Move cursor to the correct position for calibrated voltage
        voltage(volt); 
        //adc2();
                 // Display calculated voltage on the LCD
    }
}

void init() {
    TRISA = 0x05;               // Configure RA0 as input (AN0)
    TRISC = 0x00;               // Configure PORTC as output
    TRISD = 0x00;               // Configure PORTD as output
    ADCON0 = 0x81;
    ADCON1 = 0x80;              // Configure VREF+ as VDD, VREF- as VSS
    CCP1CON=0x0C;//set the pwm mode 0000 11xx(00)
    T2CON=0x06;//activate the timer prescaller 16 0000 0110
    PR2=0x5D;//92.75~93 0101 1101
    
    command(0x38);              // LCD function set: 8-bit, 2-line, 5x8 dots
    __delay_ms(10);
    command(0x0C);              // LCD display ON, cursor OFF
    __delay_ms(10);
    command(0x01);              // Clear the LCD display
    __delay_ms(10);
}
// Function to send data to the LCD
void data(unsigned int i) {
    PORTC |= 0x08;              // RS = 1 for data
    PORTD = i;                  // Send data to PORTD
    PORTC |= 0x01;              // Enable = 1
    __delay_ms(10);
    PORTC &= ~0x01;             // Enable = 0
    __delay_ms(10);
}
// Function to send commands to the LCD
void command(unsigned char i) {
    PORTC &= ~0x08;             // RS = 0 for command
    PORTD = i;                  // Send command to PORTD
    PORTC |= 0x01;              // Enable = 1
    __delay_ms(10);
    PORTC &= ~0x01;             // Enable = 0
    __delay_ms(10);
}