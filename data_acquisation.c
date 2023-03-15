//main file of data acquisation system

#include<lpc21xx.h>
#include<stdio.h>
#include"common_define.h"
#include"common_delay.h"
#include"i2c_comm_pro.c"
#include"i2c_defines.h"
#include"SPI_define.h"
#include"SPI_define.h"
#include"spi_lm35_protocol.c"
#include"lcd_display_unit.c"
#include"uart_header.h"
int main()
{
	f T;
	uc *s,i;
	uc temp;
	uc hr=0;
	uc min=0;
	uc sec=0;
	i2c_init();
	SPI_init();
	uart0_init();
	LCD_init();
	cgram_write();
	i2c_rtc_write(RTC_addr,0X02,0x20);
	i2c_rtc_write(RTC_addr,0X01,0x12);
	i2c_rtc_write(RTC_addr,0X00,0x30);	
	while(1)
	{		
			lcd_cmd(0x01);
			lcd_cmd(0x02);
			lcd_cmd(0x80);
			lcd_string("Time:");
			uart0_string_tx("Time:");
			hr = i2c_rtc_read(RTC_addr,0X02);
			lcd_data(hr/16+48);
			lcd_data(hr%16+48);
			lcd_data(':');
			uart0_tx(hr/16+48);
			uart0_tx(hr%16+48);
			uart0_tx(':');		
//			delay_ms(2);
			min = i2c_rtc_read(RTC_addr,0X01);
			lcd_data(min/16+48);
			lcd_data(min%16+48);
			lcd_data(':');
			uart0_tx(min/16+48);
			uart0_tx(min%16+48);
			uart0_tx(':');
//			delay_ms(2);			
			sec = i2c_rtc_read(RTC_addr,0X00);
			lcd_data(sec/16+48);
			lcd_data(sec%16+48);
			uart0_tx(sec/16+48);
			uart0_tx(sec%16+48);
			temp = i2c_rtc_read(RTC_addr,0X02);
			if(temp > 12)
			{
				lcd_string(" pm");
				uart0_string_tx(" pm");
			}
			else
			{
				lcd_string(" am");
				uart0_string_tx(" am");
			}
			uart0_string_tx("\r\n");
			lcd_cmd(0xc0);
			lcd_string("Temp:");
			uart0_string_tx("Temp:");
			T = spi_sensor_read(0);
			lcd_float(T);
			s = (uc *)(&T);
			for(i=0;i<sizeof(float);uart0_tx(*(s+i)),i++);
			
			lcd_data(0);
			lcd_data('c');
			uart0_tx('c');
			uart0_string_tx("\r\n");			
			delay_sec(1);	
	}
}
