#include<lpc21xx.h>
//#include"common_define.h"
#include"i2c_defines.h"

void start(void)
{
	I2CONSET = 1<<STA;
	while(((I2CONSET>>SI)&1)==0);
	I2CONCLR = 1<<STA;
}
void write(uc data)
{
	I2DAT = data;
	I2CONCLR = 1<<SI;
	while(((I2CONSET>>SI)&1)==0);
}
void restart(void)
{
	I2CONSET = 1<<STA;
	I2CONCLR = 1<<SI;
	while(((I2CONSET>>SI)&1)==0);
	I2CONCLR = 1<<STA;
}
void stop(void)
{
	I2CONSET = 1<<STO;
	I2CONCLR = 1<<STA;
}
uc nack(void)
{
	I2CONSET = 0x00;
	I2CONCLR = 1<<SI;
	while(((I2CONSET>>SI)&1)==0);
	return I2DAT;
}
uc master_ack(void)
{
	I2CONSET = 1<<AA;
	I2CONCLR = 1<<SI;
	while(((I2CONSET>>SI)&1)==0);
	I2CONCLR = 1<<AA;
	return I2DAT;
}

void i2c_init(void)
{
	//SDA and SCL pin selection
	PINSEL0 |= SCL|SDA;
	//Set the speed as 100khz
	I2SCLL = loadval;
	I2SCLH = loadval;
	//enable the i2c communicaion 
	I2CONSET = 1<<I2EN;
}
//write rtc value..............................................
void i2c_rtc_write(uc wslave_addr,uc wreg_addr,uc data)
{
	start();
	write(wslave_addr<<1);
	write(wreg_addr);
	write(data);
	stop();
	delay_ms(10);
}
//read rtc value...............................................
uc i2c_rtc_read(uc rslave_addr,uc rreg_addr)
{
	uc ret;
	start();
	write(rslave_addr<<1);
	write(rreg_addr);
	restart();
	write((rslave_addr<<1)|1);
	ret = nack();
	stop();
	delay_ms(10);
	return ret;
}
