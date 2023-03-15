//uart0 serial communication header file

//uart0 pin selection.......
#define TXD0 0x00000001  
#define RXD0 0x00000004

//uart0 baudrate calculation....
//U0DLL and U0DLM register are used to set the baudrate
#define cry_osc 12000000
#define Cpu_clk cry_osc*5
#define Per_clk Cpu_clk/4
#define baudRate 9600
#define reg_val (Per_clk/(16*baudRate))

//uart0 initialization ....

void uart0_init(void)
{
	PINSEL0 |= TXD0|RXD0;
	U0LCR = 0X83;
	U0DLL = reg_val;
	U0DLM = reg_val>>8;
	U0LCR = 0X03;	
}

void uart0_tx(uc data)
{
	while((U0LSR&(1<<6))==0);
	U0THR = data;
}

void uart0_string_tx(char *d)
{
	while(*d)
	{
			uart0_tx(*d++);
	}
}

