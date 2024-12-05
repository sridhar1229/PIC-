#include <xc.h>

// Configuration bits for PIC16F877A
#pragma config FOSC = HS        // High-Speed Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Disable
#pragma config PWRTE = OFF      // Power-up Timer Disable
#pragma config BOREN = ON       // Brown-out Reset Enable
#pragma config LVP = OFF        // Low Voltage Programming Disable
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection
#pragma config WRT = OFF        // Flash Program Memory Write Enable Disable
#pragma config CP = OFF         // Flash Program Memory Code Protection

#define _XTAL_FREQ 20000000     // 20 MHz oscillator frequency

// Define RTS/CTS using PORT registers
#define RTS PORTBbits.RB1       // Request to Send (output to receiver)
#define CTS PORTBbits.RB0       // Clear to Send (input from receiver)

// Define XON and XOFF
#define XON  0x11  // ASCII CTRL-Q
#define XOFF 0x13  // ASCII CTRL-S

void USART_Init(long baud_rate) {
    long brg_value = ((_XTAL_FREQ / (64 * baud_rate)) - 1);
    if (brg_value > 255) {
        while (1);  // Trap if baud rate is too high
    }
    SPBRG = (unsigned char)brg_value;  // Assign baud rate
    TXSTAbits.BRGH = 0;           // Low-speed mode
    TXSTAbits.SYNC = 0;           // Asynchronous mode
    RCSTAbits.SPEN = 1;           // Enable serial port

    // Set TRIS (data direction registers)
    TRISCbits.TRISC6 = 0;         // TX pin as output
    TRISCbits.TRISC7 = 1;         // RX pin as input
    TRISBbits.TRISB0 = 1;         // CTS as input
    TRISBbits.TRISB1 = 0;         // RTS as output

    TXSTAbits.TXEN = 1;           // Enable transmitter
    RCSTAbits.CREN = 1;           // Enable continuous reception
    RTS = 0;                      // PIC is not ready to receive
}

void USART_Write(char data) {
    while (CTS == 0);             // Wait until CTS is high (ready to send)
    TXREG = data;                 // Transmit data
    while (!TXSTAbits.TRMT);      // Wait for transmission to complete
}

void USART_Write_Text(const char* text) {
    while (*text != '\0') {
        USART_Write(*text);
        text++;
    }
}

char USART_Read() {
    while (!PIR1bits.RCIF);       // Wait until data is received
   
    char received = RCREG;    // Read received data
    
    if (received == XOFF) {   // If XOFF is received, pause sending
        // Pause the transmitter (user-defined logic)
        TXSTAbits.TXEN = 0;   // Disable transmitter temporarily
    } else if (received == XON) { // If XON is received, resume sending
        TXSTAbits.TXEN = 1;   // Re-enable transmitter
    }
    return RCREG;                 // Return received data
}

void Receive_Data() {
    RTS = 1;                      // Signal PIC is ready to receive data
    char received = USART_Read(); // Read received character
    RTS = 0;                      // Signal PIC is not ready (buffer is full)
    
    // Process the received data (e.g., echo it back)
    USART_Write(received);
}

void main() {
    // Initialize USART with baud rate 9600
    USART_Init(9600);

    // Example: Continuously transmit and receive data
    while (1) {
        // Send a message
        USART_Write_Text("Hello from PIC!\r\n");
        
        // Receive and process incoming data
        Receive_Data();
        
        __delay_ms(1000);  // Delay for 1 second
    }
}
