#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 6000000

void SPI_Master_Init();
void SPI_Write(uint8_t);

void main(void)
{
    TRISC = 0x10;
    PORTC = 0x81;
  //--[ Peripherals & IO Configurations ]--
  SPI_Master_Init(); // Initialize The SPI in Master Mode @ Fosc/64 SCK
  uint8_t Data = 0; // The Data Byte
  OPTION_REG = 0x7F;
  TRISB = 0x07; // RB0, RB1 & RB2 Are Input Pins (Push Buttons)
  TRISD = 0x00; // Output Port (4-Pins)
  PORTD = 0x00; // Initially OFF
  PORTB = 0xF8;
  //---------------------------
  while(1)
  {
      switch(PORTB)
      {
          case 0xFE:
              Data++;
              while(PORTB == 0xFE);
              break;
          case 0xFD:
              Data--;
              while(PORTB == 0xFD);
              break;
          case 0xFB:
              PORTC &= 0xFE;
              SPI_Write(Data);
//              __delay_ms(250);
              while(PORTB == 0xFB);
              PORTC |=0x01;
              break;
          case 0xF7:
              PORTC &= 0x7F;
               SPI_Write(Data);
//              __delay_ms(250);
              while(PORTB == 0xF7);
              PORTC |= 0x80;
              break;
      }
              
              
    PORTD = Data; // Display The Current Data Value @ PORTD
  }
}
//-------------------------------
// Functions Definitions
 
void SPI_Master_Init()
{
    SSPCON = 0x20;//SSPM0,SSMP1,SSPM2,SSPM3 = 0 SO FOSC/4 AND CKP =0 SSPEN = 1
    SSPSTAT = 0;// CKE = 0 AND SMP =0
  
}
 
void SPI_Write(uint8_t Data)
{
  SSPBUF = Data; 
}