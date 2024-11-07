#include <xc.h>
#include "EEPROM_FUNCTION.h"
#define _XTAL_FREQ 6000000
#define address_w 0xA0
#define address_r 0xA1

void master_init(const long int baud)
{
    SSPCON = 0b00101000;  // SSPEN enabled, Master mode
    SSPCON2 = 0;
    SSPADD = (unsigned char)((_XTAL_FREQ / (4 * baud)) - 1);
    SSPSTAT = 0;
    TRISC3 = 1;   // SCL as input
    TRISC4 = 1;   // SDA as input
}

void master_wait()
{
    while((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));  // Wait until bus is idle
}

void master_start()
{
    master_wait();
    SEN = 1;  // Initiate start condition
}

void repeated_start()
{
    master_wait();
    RSEN = 1;  // Initiate repeated start condition
}

void master_stop()
{
    master_wait();
    PEN = 1;  // Initiate stop condition
}

unsigned char master_write(unsigned char data)
{
    master_wait();
    SSPBUF = data;  // Send data
    while(!SSPIF);  // Wait until transmission is complete
    SSPIF = 0;
    return ACKSTAT;  // Return ACK status
}

unsigned char read_byte()
{
    master_wait();
    RCEN = 1;   // Enable receive mode
    while(!SSPIF);
    SSPIF = 0;
    master_wait();
    return SSPBUF;  // Return received data
}



void nack()
{
    ACKDT = 1;  // Not Acknowledge bit
    master_wait();
    ACKEN = 1;  // Enable Not Acknowledge
}

unsigned char read(unsigned int adr)
{
    unsigned char data;
    master_start();
    while(master_write(address_w))  // Write address with write bit
        repeated_start();  // Retry if not acknowledged we should use this after read and write

    while(master_write(adr >> 8))        // Send high byte of address
      {
        master_stop();
        master_start();  // Retry if not acknowledged
        
    }
    while(master_write((unsigned char)adr)) // Send low byte of address
          {
        master_stop();
        master_start();  // Retry if not acknowledged
        
    }
    repeated_start();
    
    while(master_write(address_r))  // Write address with read bit
     {
         master_stop();
        master_start();  // Retry if not acknowledged
       
    }
    data = read_byte();
    nack();                   // No further data
    master_stop();            // Stop condition
    
    return data;
}

void my_eeprom_write(unsigned int adr, unsigned char data)
{
    master_start();
    while(master_write(address_w))  // Write address with write bit
        repeated_start();  // Retry if not acknowledged
    
// the master is stop ans=d then start so it repeat the process
    
    while(master_write(adr >> 8))         // Send high byte of address
    {
        master_stop();
        master_start();  // Retry if not acknowledged
        
    }
    while(master_write((unsigned char)adr)) // Send low byte of address
      {
        master_stop();  // Retry if not acknowledged
        master_start();
    }
    while(master_write(data))             // Send data byte
      {
        master_stop();
        master_start();  // Retry if not acknowledged
        
    }
    master_stop();
    __delay_ms(10);                  // Delay to allow EEPROM to complete write
}


