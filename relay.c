

#include <pic.h>

void main(void) {
    TRISB0 = 1;
    TRISB1 = 0;
    while(1)
    {
        if(RB0==1)
        {
            RB1=1;
        }
        else
        {
            RB1=0;
        }
    }   
}
