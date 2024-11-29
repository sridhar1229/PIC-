#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 6000000   // 6MHz Crystal Oscillator Frequency

// Function Prototypes
void init();
void master_init(const unsigned long);
void master_write(unsigned char);
unsigned int master_read(unsigned int);
void master_start();
void master_wait();
void master_stop();
void set(void);
void update(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
unsigned int bcd_2_dec(unsigned int);
unsigned int dec_2_bcd(unsigned int);
unsigned int am_pm(unsigned int);

// Global Variables
unsigned char msg1[6] = {"TIME:"};
unsigned char msg2[6] = {"DATE:"};
unsigned char msg3[3] = {"AM"};
unsigned char msg4[3] = {"PM"};
unsigned int sec = 55, min = 59, hour = 11;
unsigned int date = 18, month = 11, year = 24;
unsigned int sec1, sec2, min1, min2, hour1, hour2;
unsigned int date1, date2, month1, month2, year1, year2;

// Main Function
void main() {
    init();                      // Initialize ports and LCD
    master_init(100);            // Initialize I2C Master mode at 100kHz
    set();                       // Set initial RTC time and date
    while (1) {
        update();                // Read current time and date from RTC

        // Split time into digits for display
        sec1 = sec / 10; sec2 = sec % 10;
        min1 = min / 10; min2 = min % 10;
        hour1 = hour / 10; hour2 = hour % 10;
        date1 = date / 10; date2 = date % 10;
        month1 = month / 10; month2 = month % 10;
        year1 = year / 10; year2 = year % 10;

        // Display time on the first line of the LCD
        lcd_command(0x80);         // Move cursor to the first line
        for (int i = 0; i < 5; i++) lcd_data(msg1[i]);  // Display "TIME:"
        lcd_data(hour1 + '0'); lcd_data(hour2 + '0');
        lcd_data(':');             // Display ':'
        lcd_data(min1 + '0'); lcd_data(min2 + '0');
        lcd_data(':');             // Display ':'
        lcd_data(sec1 + '0'); lcd_data(sec2 + '0');

        // Display date on the second line of the LCD
        lcd_command(0xC0);         // Move cursor to the second line
        for (int i = 0; i < 5; i++) lcd_data(msg2[i]);  // Display "DATE:"
        lcd_data(date1 + '0'); lcd_data(date2 + '0');
        lcd_data('/');             // Display '/'
        lcd_data(month1 + '0'); lcd_data(month2 + '0');
        lcd_data('/');             // Display '/'
        lcd_data(year1 + '0'); lcd_data(year2 + '0');

        __delay_ms(500);           // Small delay for stability
    }
}

// Function Definitions

// Initialize ports and LCD
void init() {
    TRISD = 0x00;   // Set PORTD as output for LCD data
    TRISC = 0x18;   // Set RC3 and RC4 as input for I2C, others as output
    PORTD = 0x00;   // Clear PORTD

    // LCD Initialization
    lcd_command(0x30); __delay_ms(10);
    lcd_command(0x30); __delay_ms(10);
    lcd_command(0x30); __delay_ms(10);
    lcd_command(0x38); __delay_ms(10); // 8-bit mode, 2-line display
    lcd_command(0x06); __delay_ms(10); // Entry mode: Increment cursor
    lcd_command(0x0C); __delay_ms(10); // Display ON, Cursor OFF
    lcd_command(0x01); __delay_ms(10); // Clear display
}

// Send a command to the LCD
void lcd_command(unsigned char cmd) {
    PORTC &= ~0x02; // RS = 0 for command
    PORTD = cmd;    // Send command to PORTD
    PORTC |= 0x01;  // Enable = 1
    PORTC &= ~0x01; // Enable = 0
    __delay_ms(10);
}

// Send data to the LCD
void lcd_data(unsigned char data) {
    PORTC |= 0x02;  // RS = 1 for data
    PORTD = data;   // Send data to PORTD
    PORTC |= 0x01;  // Enable = 1
    PORTC &= ~0x01; // Enable = 0
    __delay_ms(10);
}

// Initialize I2C Master mode
void master_init(const unsigned long freq) {
    SSPCON = 0x28;  // Enable SSPEN, select Master mode
    SSPSTAT = 0x00; // Clear SSPSTAT
    SSPCON2 = 0x00; // Clear SSPCON2
    SSPADD = (_XTAL_FREQ / (4 * freq * 100)) - 1; // Set I2C speed
}

// Wait for I2C to become idle
void master_wait() {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

// Generate an I2C Start condition
void master_start() {
    master_wait();
    SEN = 1;  // Start Enable bit
}

// Generate an I2C Stop condition
void master_stop() {
    master_wait();
    PEN = 1;  // Stop Enable bit
}

// Write a byte to the I2C bus
void master_write(unsigned char data) {
    master_wait();
    SSPBUF = data;  // Load data into buffer
}

// Read a byte from the I2C bus
unsigned int master_read(unsigned int ack) {
    unsigned int value;
    master_wait();
    RCEN = 1;       // Enable Receive
    master_wait();
    value = SSPBUF; // Read received byte
    master_wait();
    ACKDT = (ack) ? 0 : 1; // ACK or NACK
    ACKEN = 1;      // Send acknowledge bit
    return value;
}

// Convert Decimal to BCD
unsigned int dec_2_bcd(unsigned int val) {
    return ((val / 10) << 4) + (val % 10);
}

// Convert BCD to Decimal
unsigned int bcd_2_dec(unsigned int val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

// Handle AM/PM and convert to Decimal
unsigned int am_pm(unsigned int val) {
    if (val & 0x60) {       // Check for PM
        lcd_command(0x8D);
        for (int i = 0; i < 2; i++) lcd_data(msg4[i]); // Display "PM"
        val &= ~0x60;       // Clear PM bit
    } else {
        lcd_command(0x8D);
        for (int i = 0; i < 2; i++) lcd_data(msg3[i]); // Display "AM"
    }
    return bcd_2_dec(val);  // Convert to Decimal
}

// Set initial time and date in RTC
void set() {
    master_start();
    master_write(0xD0);  // RTC Slave Address with Write bit
    master_write(0x00);  // Start at register 0
    master_write(dec_2_bcd(sec));  // Set Seconds
    master_write(dec_2_bcd(min));  // Set Minutes
    master_write(0x60 | dec_2_bcd(hour));  // Set Hours with PM bit
    master_write(0x01);  // Set Day (not used here)
    master_write(dec_2_bcd(date));  // Set Date
    master_write(dec_2_bcd(month)); // Set Month
    master_write(dec_2_bcd(year));  // Set Year
    master_stop();
}

// Update time and date by reading from RTC
void update() {
    master_start();
    master_write(0xD0);        // RTC Slave Address with Write bit
    master_write(0x00);        // Start at register 0
    master_stop();
    master_start();
    master_write(0xD1);        // RTC Slave Address with Read bit

    sec = bcd_2_dec(master_read(1));   // Read Seconds
    min = bcd_2_dec(master_read(1));   // Read Minutes
    hour = am_pm(master_read(1));      // Read Hours and handle AM/PM
    master_read(1);                    // Read Day (not used here)
    date = bcd_2_dec(master_read(1));  // Read Date
    month = bcd_2_dec(master_read(1)); // Read Month
    year = bcd_2_dec(master_read(0));  // Read Year (send NACK)
    master_stop();
}
