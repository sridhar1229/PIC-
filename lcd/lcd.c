#include <xc.h>
#define _XTAL_FREQ 20000000
#define RS RD0
#define RW RD1
#define EN RD2

void lcd_data(unsigned char data)
{
    PORTC = data;
    RS = 1; // RS = 1 for data
    RW = 0; // RW = 0 for write
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char command)
{
    PORTC = command;
    RS = 0; // RS = 0 for command
    RW = 0; // RW = 0 for write (corrected)
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const unsigned char *string, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
    {
        lcd_data(string[i]);
    }
}

void lcd_initialize()
{
    __delay_ms(20); // Wait for 20ms after power on
    lcd_command(0x38); // Function set: 8-bit, 2-line, 5x8 dots
    __delay_ms(5); // Wait for more than 4.1ms
    lcd_command(0x38); // Repeat function set
    __delay_us(200); // Wait for more than 100us
    lcd_command(0x38); // Repeat function set

    lcd_command(0x06); // Entry mode set: increment automatically, no shift
    lcd_command(0x0C); // Display on, cursor off, no blink
    lcd_command(0x01); // Clear display
    __delay_ms(2); // Wait for more than 1.53ms
}

void main(void) 
{
    TRISC = 0x00; // PORTC as output
    TRISD = 0x00; // PORTD as output
    lcd_initialize();
    while(1)
    {
        lcd_command(0x83); // Set cursor to beginning of first line
        lcd_string("EMBEDDED", 8);
        lcd_command(0xC3); // Set cursor to beginning of second line
        lcd_string("SYSTEMS", 7);
    }
}
