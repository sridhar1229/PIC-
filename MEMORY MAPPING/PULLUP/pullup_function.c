unsigned char *internal_pullup = (unsigned char*)0x81;
unsigned char *dir_b = (unsigned char*)0x86;
unsigned char *dir_d = (unsigned char*)0x88 ;
unsigned char *value_b = (unsigned char*)0x06 ;
unsigned char *value_d = (unsigned char*)0x08 ;

void init_internalpullup()
{
    *internal_pullup &= 0x7F;
}

void init_button()
{
    *dir_b = 0xFF;
}

void init_led()
{
    *dir_d = 0x00;
    *value_d =0x00;
}

void led_on(unsigned char i)
{
    switch(i)
    {
        case 1:
            *value_d = 0x08;
            break;
        case 2:
            *value_d= 0x20;
            break;
        case 3:
            *value_d = 0x28;
            break;
        case 0:
            *value_d = 0x00;
            break;
            
    }  
}

unsigned int button(void)
{
     return *value_b  & 0xFF;
}
    
    


