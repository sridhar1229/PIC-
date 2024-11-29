#pragma config WDTE = OFF    // Watchdog Timer disabled
#pragma config PWRTE = ON    // Power-up Timer enabled
#pragma config BOREN = ON    // Brown-out Reset enabled
#pragma config LVP = OFF     // Low-Voltage Programming disabled
#pragma config CPD = OFF     // Data EEPROM Code Protection off
#pragma config WRT = OFF     // Flash Program Memory Write Protection off
#pragma config CP = OFF      // Flash Program Memory Code Protection off

// Include header files
#include <xc.h>              // XC8 Compiler header file for PIC microcontrollers

#define _XTAL_FREQ 6000000   // Define the operating frequency for delay functions
#define RS RC2               // Register Select pin for LCD
#define EN RC1               // Enable pin for LCD

// Function Prototypes
void init(void);
void master_init(const unsigned long); // Initialize I2C Master
void master_write(unsigned char);      // Write data to I2C bus
int master_read(int);                  // Read data from I2C bus
void master_start();                   // Start I2C communication
void master_wait();                    // Wait for I2C conditions to clear
void master_stop();                    // Stop I2C communication
void lcd_command(unsigned char);       // Send command to LCD
void lcd_data(unsigned char);          // Send data to LCD
int bcd_2_dec(int);                    // Convert BCD to decimal
int dec_2_bcd(int);                    // Convert decimal to BCD
void set(void);                        // Set initial RTC values
void update(void);                     // Update RTC values

// LCD Display Messages
char msg1[5] = {"TIME:"};              // Message for time display
char msg2[5] = {"DATE:"};              // Message for date display

// Variables for RTC data
int sec = 55;    // Initial seconds
int min = 59;    // Initial minutes
int hour = 11;   // Initial hours
int date = 28;   // Initial date
int month = 12;  // Initial month
int year = 20;   // Initial year

// Variables for LCD character representation
char sec1, sec2, min1, min2, hour1, hour2, date1, date2, month1, month2, year1, year2;

void main() {
    init();              // Initialize LCD and peripherals
    master_init(100);    // Initialize I2C master with 100 kHz frequency
    set();               // Set initial RTC values

    while(1) {
        update();        // Update RTC values from I2C
        
        // Split time and date values into individual digits
        sec1 = sec / 10;
        sec2 = sec % 10;
        min1 = min / 10;
        min2 = min % 10;
        hour1 = hour / 10;
        hour2 = hour % 10;
        date1 = date / 10;
        date2 = date % 10;
        month1 = month / 10;
        month2 = month % 10;
        year1 = year / 10;
        year2 = year % 10;

        // Display "TIME:" on the LCD
        lcd_command(0x80);  // Move cursor to the first line
        for(int i = 0; i < 5; i++) 
            lcd_data(msg1[i]);
        // Display time in HH-MM-SS format
        lcd_data(hour1 + '0');
        lcd_data(hour2 + '0');
        lcd_data(0x2D);  // Display '-'
        lcd_data(min1 + '0');
        lcd_data(min2 + '0');
        lcd_data(0x2D);
        lcd_data(sec1 + '0');
        lcd_data(sec2 + '0');

        // Display "DATE:" on the LCD
        lcd_command(0xC0);  // Move cursor to the second line
        for(int i = 0; i < 5; i++) 
            lcd_data(msg2[i]);
        // Display date in DD-MM-YY format
        lcd_data(date1 + '0');
        lcd_data(date2 + '0');
        lcd_data(0x2D);
        lcd_data(month1 + '0');
        lcd_data(month2 + '0');
        lcd_data(0x2D);
        lcd_data(year1 + '0');
        lcd_data(year2 + '0');

        __delay_ms(500);  // Refresh every 500ms
    }
}

// Initialize LCD and I/O settings
void init(void) {
    TRISD = 0x00;    // Set PORTD as output for LCD data
    TRISC = 0x18;    // Set RC3 and RC4 as input for I2C
    PORTD = 0x00;    // Clear PORTD
    // Initialize LCD commands
    lcd_command(0x38);  // Function set: 8-bit, 2-line, 5x8 dots
    __delay_ms(5);
    lcd_command(0x0C);  // Display ON, Cursor OFF
    __delay_ms(5);
    lcd_command(0x06);  // Entry mode: Auto-increment
    __delay_ms(5);
    lcd_command(0x01);  // Clear display
    __delay_ms(5);
}

// Send command to LCD
void lcd_command(unsigned char i) {
    RS = 0;        // Command mode
    PORTD = i;     // Send command
    EN = 1;        // Enable pulse
    EN = 0;
    __delay_ms(5);
}

// Send data to LCD
void lcd_data(unsigned char i) {
    RS = 1;        // Data mode
    PORTD = i;     // Send data
    EN = 1;        // Enable pulse
    EN = 0;
    __delay_ms(5);
}

// Initialize I2C Master
void master_init(const unsigned long feq_k) {
    SSPCON = 0x28;  // Enable I2C master mode
    SSPSTAT = 0x00; // Clear SSP status
    SSPCON2 = 0x00; // Clear SSP control
    SSPADD = (_XTAL_FREQ / (4 * feq_k * 100)) - 1; // Set clock frequency
}

// Wait for I2C operation to complete
void master_wait() {
    while(SSPCON2 & 0x1F || SSPSTAT & 0x04);
}

// Generate I2C start condition
void master_start() {
    master_wait();
    SEN = 1;
}

// Generate I2C stop condition
void master_stop() {
    master_wait();
    PEN = 1;
}

// Write data to I2C bus
void master_write(unsigned char temp) {
    master_wait();
    SSPBUF = temp;  // Write data to buffer
}

// Read data from I2C bus
int master_read(int ack) {
    int value;
    master_wait();
    RCEN = 1;  // Enable receive mode
    master_wait();
    value = SSPBUF;  // Read received data
    master_wait();
    ACKDT = (ack) ? 0 : 1;  // Acknowledge bit
    ACKEN = 1;
    return value;
}

// Convert decimal to BCD
int dec_2_bcd(int temp) { 
    return ((temp / 10) << 4) + (temp % 10);
}

// Convert BCD to decimal
int bcd_2_dec(int temp) { 
    return ((temp >> 4) * 10) + (temp & 0x0F);
}

// Set initial RTC values
void set(void) {
    master_start();
    master_write(0xD0);           // RTC write address
    master_write(0);              // Start at seconds register
    master_write(dec_2_bcd(sec)); // Set seconds
    master_write(dec_2_bcd(min)); // Set minutes
    master_write(dec_2_bcd(hour));// Set hours
    master_write(1);              // Skip day register
    master_write(dec_2_bcd(date));// Set date
    master_write(dec_2_bcd(month));// Set month
    master_write(dec_2_bcd(year));// Set year
    master_stop();
}

// Update RTC values from I2C
void update(void) {
    master_start();
    master_write(0xD0);  // RTC write address
    master_write(0);     // Start at seconds register
    master_stop();

    master_start();
    master_write(0xD1);  // RTC read address
    sec = bcd_2_dec(master_read(1)); // Read seconds
    min = bcd_2_dec(master_read(1)); // Read minutes
    hour = bcd_2_dec(master_read(1));// Read hours
    master_read(1);                   // Skip day register
    date = bcd_2_dec(master_read(1));// Read date
    month = bcd_2_dec(master_read(1));// Read month
    year = bcd_2_dec(master_read(1)); // Read year
    master_stop();
}
