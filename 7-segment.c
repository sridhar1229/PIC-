
#include <pic.h>
#define _XTAL_FREQ 20000000
void main(void) {
    TRISC=0x00;
    unsigned int s[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    while(1){
    for(int i=0;i<10;i++){
        PORTC= s[i];
        __delay_ms(1000);
    }
    }
 
}
