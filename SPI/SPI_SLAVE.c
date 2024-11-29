#pragma config FOSC = HS     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON     // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <stdint.h>


void SPI_Slave_Init();
uint8_t SPI_Read();

uint8_t Data; // Global Received Data Variable (Not Recommended)

void main(void)
{
    SPI_Slave_Init();
    TRISD = 0x00; // Configure PORTB as output
    PORTD = 0x00;
    while (1)
    {
        // Dumping Received Data To PORTB To Validate Communication Success!
        PORTD = Data;
    }
    
}

void SPI_Slave_Init()
{
    // Set SPI Mode to Slave with SS Enabled
    SSPCON = 0x24; // SSPEN = 1 (Enable SPI), Slave mode with SS enabled, CKP = 0
    
    // Configure SPI Clock Polarity and Phase (SPI Mode 1)
    SSPSTAT = 0x00; // CKE = 0 (Transmit on rising edge), SMP = 0 (Data sampled at the middle)

    // Configure I/O Pins for SPI
    TRISC = 0x18; // RC5 (SDO) = Output, RC4 (SDI) = Input, RC3 (SCK) = Input
    TRISA = 0x20; // RA5 (SS) = Input

    // Configure ADCON1 to Set RA5 (SS) as Digital I/O
    ADCON1 = 0x0E; // RA5 = Digital, AN0 remains analog

    // Enable Interrupts
    PIE1 = 0x08;  // SSPIE = 1 (Enable SPI interrupt)
    INTCON = 0xC0; // GIE = 1 (Global Interrupt Enable), PEIE = 1 (Peripheral Interrupt Enable)
}

uint8_t SPI_Read() // Not Recommended Method To Read SPI Data
{
    if (SSPSTAT & 0x01) // Check if Buffer-Full (BF) is set
    {
        Data = SSPBUF;      // Read the Buffer
        SSPCON &= ~0x40;    // Clear Overflow Indicator (SSPOV = 0)
        PIR1 &= ~0x08;      // Clear SSP Interrupt Flag (SSPIF = 0)
        
    }
    return Data;
}

void __interrupt()ISR(void)
{
    if (PIR1 & 0x08) // Check if SSP Interrupt Flag (SSPIF) is set
    {
        //SPI_Read();
        Data = SSPBUF;      // Read the Buffer
        PIR1 &= ~0x08;      // Clear SSP Interrupt Flag (SSPIF = 0)
    }
}
