unsigned char *dir_value =(unsigned char*)0x87;
unsigned char *port_value =(unsigned char*)0x07;

    void led_init()
    {
        *dir_value = 0x00;
    }
    void led_on()
    {
        *port_value = 0xFF;
    }
    void led_off()
    {
        *port_value = 0x00;
    }
   

