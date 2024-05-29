#include <xc.h>
#define _XTAL_FREQ 20000000

void main(void) {
    TRISB0 = 0;
    while(1){
        RB0 = 1;
        __delay_ms(1000);
        RB0 = 0;
        __delay_ms(1000);
    }
}
