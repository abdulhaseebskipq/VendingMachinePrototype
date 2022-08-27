#define SYSCTL_RCGCGPIO_R  (*((volatile unsigned long *)0x400FE608))
#define NVIC_EN0_R (*((volatile unsigned long *)0xE000E100))
#define NVIC_PR17_R (*((volatile unsigned long *)0xE000E41C))
#define GPIO_PORTF_DATA_R   (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R   (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R   (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_PUR_R   (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_IS_R   (*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R   (*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R   (*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_IM_R   (*((volatile unsigned long *)0x40025410))
#define GPIO_PORTF_ICR_R   (*((volatile unsigned long *)0x4002541C))

#define NVIC_EN0_INT30    0x40000000;
#define PORTF_CLK_EN      0x23     //Enabling port A, B ,F
#define LEDs              0x0E
#define SW1               0x10
#define INT_PF4           0x10

// MAIN defines

#include "tm4c123gh6pm.h"
#define LCDDATA (*((volatile uint32_t*)0x400053FC))   // PORTB
#define LCDCMD (*((volatile uint32_t*)0x40004300))    // PA7-PA6
#define lcd_display_data_1  "    Welcome To  Vending Machine"
#define lcd_display_data_3  " 1)Item1 - 5$   "
#define lcd_display_data_4  " 2)Item2 - 10$  "
#define lcd_display_data_5  " tu mera putr   "
#define lcd_display_data_6  " chuti kar      "
#define lcd_display_data_7  " Select Amount: "
#define lcd_display_data_8  " 1) 5$    2) 10$"
#define lcd_display_data_9  " Payment Done   "
#define lcd_display_data_10 " item delivered "
#define lcd_display_data_11 " returned 5$    "
#define lcd_display_data_12 " not enough cash"
#define lcd_display_data_13 " reselect       "
#define lcd_display_data_14 " 1) Refund      "
#define lcd_display_data_15 " 2) Repay       "
#define lcd_display_data_16 " refunded cash  "
#define lcd_display_data_17 "   Thanks   for    Shopping ! "


#define E  0x80 // on PA7
#define RS 0x40 // on PA6
int string_iterative;
int item;
int payment=0;
int success=0;


char array_text[1];
void float_text (char str [],unsigned int length );
void setup (void);
void LCD_Init(void);
void delay(unsigned long value);
void OutCmd(unsigned char command);
void LCD_OutChar(unsigned char letter);
void LCD_Clear(void);
void lcd_DisplayString (char str [],unsigned int length);
void switch_defines(void);
void display_two_lines(char str1 [],char str2 []);
int payment_function(int item, int pay);



// MAIN defines


void EnableInterrupts(void);
void DisableInterrupts(void);
void Init_INT_GPIO(void);
void Delay(unsigned long value);
void WaitForInterrupt(void);

volatile unsigned long i = 0;

void Init_INT_GPIO(void)
{
	volatile unsigned delay_clk;
	SYSCTL_RCGCGPIO_R |= PORTF_CLK_EN;
	delay_clk = SYSCTL_RCGCGPIO_R ;
	GPIO_PORTF_DEN_R |= (SW1|LEDs) ;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_PUR_R = SW1;
  

DisableInterrupts();

	GPIO_PORTF_IS_R &= ~INT_PF4;
	GPIO_PORTF_IBE_R &= ~INT_PF4;
	GPIO_PORTF_IEV_R &= ~INT_PF4;
	GPIO_PORTF_ICR_R |= INT_PF4;
	GPIO_PORTF_IM_R |= INT_PF4;
 	
	
	NVIC_PR17_R = (NVIC_PR17_R & 0xFF00FFFF) | 0x00A00000 ;
	NVIC_EN0_R = NVIC_EN0_INT30;
	EnableInterrupts();
	
}

void Delay(unsigned long value)
{ 
	unsigned long i = 0;
	

	for(i=0;i<value;i++);

}

void GPIOPortF_Handler(void)
{
	int j;
	GPIO_PORTF_ICR_R = INT_PF4;
	
	if(i==3)
		i = 1;
	else
		i++;
	
	for(j = 0;j<2;j++)
{
	GPIO_PORTF_DATA_R ^=1<<i;
	Delay(1000000);
	
}
string_iterative=0;
LCD_Clear();
}

// INTERRUPT ABOVE





/*--------------------------------------------------/
      Name : LCD_Init
			Description : lcd initialization  
/--------------------------------------------------*/
void LCD_Init()
{
		LCDCMD = 0;           // E=0, R/W=0, RS=0
		delay (500000);       // Wait >30 ms after power is applied
		OutCmd(0x38);         // Function set: 8-bit/2-line
		delay(1000); 					// wait 40us
		OutCmd(0x0C);         // Display ON; Cursor ON
		delay(1000); 					// wait 40us
		OutCmd(0x06);         // Entry mode set
		delay(1000); 					// wait 40us
		LCD_Clear() ;
}	
/*--------------------------------------------------/
      Name : setup
			Description : Configure GPIO pins used to communicate
			with LCD and clock setup for the experiment 
/--------------------------------------------------*/
void setup (){
		// clock setup
	SYSCTL_RCGC2_R |= PORTF_CLK_EN;  // 1) activate clock for Ports A and B
  GPIO_PORTB_AMSEL_R &= ~0xFF;   // 3) disable analog function on PB7-0
  GPIO_PORTA_AMSEL_R &= ~0xC0;   //    disable analog function on PA7-6 
  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) configure PB7-0 as GPIO   
  GPIO_PORTA_PCTL_R &= ~0xFF000000; //    configure PA7-6 as GPIO
  GPIO_PORTB_DIR_R = 0xFF;       // 5) set direction register
  GPIO_PORTA_DIR_R |= 0xC0;
  GPIO_PORTB_AFSEL_R = 0x00;     // 6) regular port function
  GPIO_PORTA_AFSEL_R &= ~0xC0;
  GPIO_PORTB_DEN_R = 0xFF;       // 7) enable digital port
  GPIO_PORTA_DEN_R |= 0xC0;
  GPIO_PORTB_DR8R_R = 0xFF;      // enable 8 mA drive
  GPIO_PORTA_DR8R_R |= 0xC0;}
 /*--------------------------------------------------/
  Name : lcd data
  Description : sends the data to the lcd controller
 /--------------------------------------------------*/
void LCD_OutChar(unsigned char letter){
  LCDDATA = letter;
  LCDCMD = RS;          // E=0, R/W=0, RS=1
  delay(150);   // wait 6us
  LCDCMD = E+RS;        // E=1, R/W=0, RS=1
  delay(150);   // wait 6us
  LCDCMD = RS;          // E=0, R/W=0, RS=1
	delay(1000);   // wait 40us
}
 /*--------------------------------------------------/
			 Name : lcd cmd
			 Description : sends the command to the lcd controller
 /--------------------------------------------------*/
void OutCmd(unsigned char command)
{
  LCDDATA = command;
  LCDCMD = 0;           // E=0, R/W=0, RS=0
	delay (150);
  LCDCMD = E;           // E=1, R/W=0, RS=0
	delay(150);
  LCDCMD = 0;           // E=0, R/W=0, RS=0
	delay(1000);
}
 /*--------------------------------------------------/
 Name : clear
 Descripton : clears the entire display and set DDRAM addr ess to 0
 /--------------------------------------------------*/
 // Clear the LCD
// Inputs: none
// Outputs: none
void LCD_Clear(void)
{
  OutCmd(0x01);          // Clear Display
  delay(27000); // wait 1.6ms 
  OutCmd(0x02);          // Cursor to home
  delay(27000); // wait 1.6ms 
}
/*--------------------------------------------------/
		Name : lcd Display String
		Description : displays character string on the lcd
/--------------------------------------------------*/
 void lcd_DisplayString ( char str [] , unsigned int length )
 {
 unsigned int z ;
	for ( z =0; z<length ; z++)
	{ 
	LCD_OutChar(str [z]) ;
	}
 }
void delay(unsigned long value)
{	
	unsigned long i ;
 for ( i = 0 ; i < value ; i++);
}
/*--------------------------------------------------/
      Name : float_text  
/--------------------------------------------------*/
void float_text(char str [],unsigned int length )
{
	OutCmd(0x80);
	OutCmd(0x01);
  string_iterative=0;
	while(str[string_iterative]!='\0')
	{
		LCD_OutChar(str [string_iterative]) ;
		delay(200000);
		++string_iterative;
		if(string_iterative==15)
		{
			OutCmd(0xc0);
		}
	}
	delay(200000);
}

void switch_defines(void)
{
		GPIO_PORTF_LOCK_R=0x4C4F434B;// unlocking commit register
		GPIO_PORTF_CR_R=0x011;  //setting the commit register
		GPIO_PORTF_PUR_R=0x011; // enabling pull-up resistor for PF0 and PF4
		GPIO_PORTF_DEN_R=0x011;
}

void display_two_lines(char str1 [],char str2 [])
{
		OutCmd(0x80);         // line 1
		lcd_DisplayString (str1,16) ;
		OutCmd(0xc0);					// line 2
		lcd_DisplayString (str2,16);
		delay(10000000);
}
int payment_function(int item, int pay)
{
	if(pay==item)
	{
		display_two_lines(lcd_display_data_9,lcd_display_data_10);
		return 1;
	}
	else
	{
		if(pay<item)
		{
			display_two_lines(lcd_display_data_12,lcd_display_data_13);
			delay(1000000);
			while(1)
			{
				display_two_lines(lcd_display_data_14,lcd_display_data_15);
				if(GPIO_PORTF_DATA_R == 0x01)   //refund   switch1
				{
					display_two_lines(lcd_display_data_14,lcd_display_data_16);
					return 1;
				}
				else if(GPIO_PORTF_DATA_R == 0x10)		//repay  switch2
				{
					return 0;
				}
			}
		}
		else
		{
			display_two_lines(lcd_display_data_11,lcd_display_data_10);
			return 1;
		}
	}
}
 /*--------------------------------------------------/
										main ()
 /--------------------------------------------------*/
 int main()
 {
	
	setup() ;
	LCD_Init();
	Init_INT_GPIO();
	switch_defines();
	
	while(1)																		//main loop
	{
		
		float_text (lcd_display_data_1,26);
		WaitForInterrupt();
		DisableInterrupts();
		while(1)																	//Items loop
		{
			display_two_lines(lcd_display_data_3,lcd_display_data_4);
			if(GPIO_PORTF_DATA_R == 0x00)
			{
				display_two_lines(lcd_display_data_5,lcd_display_data_6);
			}
			else if(GPIO_PORTF_DATA_R == 0x01)   //item 1 5$   switch1
			{
				item = 5;
				break;
			}
			else if(GPIO_PORTF_DATA_R == 0x10)		//item 2 10$  switch2
			{
				item = 10;
				break;
			}
		}
		while(success==0)																	//Payment loop
		{
			display_two_lines(lcd_display_data_7,lcd_display_data_8);    //select amount
			
			if(GPIO_PORTF_DATA_R==0x01)							 //pay 5$   switch1
			{
				payment=payment+5;
				success=payment_function(item,payment);
			}
			else if(GPIO_PORTF_DATA_R==0x10)				//pay 10$   switch2
			{
				payment=payment+10;
				success=payment_function(item,payment);
			}
			
		}
		float_text(lcd_display_data_17,26);
		delay(10000000);
		payment=0;
		success=0;
		EnableInterrupts();
		
	}

}
	