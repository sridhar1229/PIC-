#include <xc.h>
#define _XTAL_FREQ 20000000
#define RS RD0
#define RW RD1
#define EN RD2

#define C1 RB0
#define C2 RB1
#define C3 RB2
#define R1 RB4
#define R2 RB5
#define R3 RB6
#define R4 RB7

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
    RW = 0; // RW = 0 for write
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

void keypad()
{
    C1 = 1; C2 = 0; C3 = 0;
    if (R1 == 1)
    {
        lcd_data('1');
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('4');
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('7');
        while (R3 == 1);
    }
    if (R4 == 1)
    {
        lcd_data('*');
        while (R4 == 1);
    }

    C1 = 0; C2 = 1; C3 = 0;
    if (R1 == 1)
    {
        lcd_data('2');
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('5');
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('8');
        while (R3 == 1);
    }
    if (R4 == 1)
    {
        lcd_data('*');
        while (R4 == 1);
    }

    C1 = 1; C2 = 0; C3 = 0;
    if (R1 == 1)
    {
        lcd_data('1');
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('4');
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('7');
        while (R3 == 1);
    }
    if (R4 == 1)
    {
        lcd_data('0');
        while (R4 == 1);
    }

    C1 = 0; C2 = 0; C3 = 1;
    if (R1 == 1)
    {
        lcd_data('3');
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('6');
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('9');
        while (R3 == 1);
    }
    if (R4 == 1)
    {
        lcd_data('#');
        while (R4 == 1);
    }
}

void main(void)
{
    TRISC = 0x00; // PORTC as output
    TRISD = 0x00; // PORTD as output
    TRISB = 0xF0; // RB4-RB7 as input, RB0-RB2 as output
    lcd_initialize();
    lcd_command(0x80);
    lcd_string((const unsigned char *)"KEYPAD:", 7); // Cast to avoid warning
    lcd_command(0xC0);
    while (1)
    {
        keypad();
    }
}
