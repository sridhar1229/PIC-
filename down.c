/*
 * File:   down.c
 * Author: thami
 *
 * Created on 30 May, 2024, 3:42 PM
 */


#include <pic.h>
#define _XTAL_FREQ 20000000
void main(void) {
    TRISB0= 1;
    TRISB1=0;
    while(1)
    {
        if(RB0==1){
            RB1=1;
        }
        else
        {
            RB1=0;
        }
    }
}
