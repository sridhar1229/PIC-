
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_led_H
#define	XC_led_H
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 
#ifdef	__cplusplus
}
#endif /* __cplusplus */
void led_init();
void led_on();
void led_off();
#endif	/* XC_HEADER_TEMPLATE_H */
