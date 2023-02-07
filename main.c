/*
 * 350Lab2.c
 *
 * Created: 2/6/2023 6:04:21 PM
 * Author : simon
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "inc/uart.h"

/*--------------------Variables---------------------------*/
char String[25];
int pulse = 0;
int spaced = 0;
char letter[5];

void Initialize();

/* need to make sure my timer modes are correct
switching between ctc and normal mode correctly */

int main(void)
{
	Initialize();
    while (1) 
    {		
		//if timer gets to 25000, record a space if necessary
		if (spaced = 0)
		{
			//if timer gets to 25000
			//record a space
			sprintf(String, "%u", letter)
			UART_putstring(String);
			sprintf(String," ");
			UART_putstring(String);
		}
		
		//clear the counter
		TCCR1B |= (1<<WGM12);	
    }
}

void Initialize()
{
	cli();
	UART_init(BAUD_PRESCALER); 
	
	TIMSK1 |= (1<<ICIE1);
	PCICR |= (1<<PCIE2);
	DDRB |= (1<<DDB5);
	
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	TCCR1B &= ~(1<<WGM10);
	TCCR1B &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	TCCR1B |= (1<<ICES1);
	
	TCCR1B |= (1<<ICF1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	TIFR1 |= (1<<ICF1);
	if (PINB & (1<<PINB0))
	{
		//turn on board LED on
		PORTB |= (1<<PORTB5);
		
		//set counter to 0
		TCCR1B &= ~(1<<TCNT0);
		
	}
	else
	{
		//turn on board LED off
		PORTB &= ~(1<<PORTB5);
		
		//record pulse and update letter
		timer = length of pulse;
		// save pulse in sequence vector

		/* start of morse code section*/
		if (sequence(0) == 1)
		{
			letter = "a";
			if  (sequence(1) == 1)
			{
				letter = "i";
				if (sequence(2) == 1)
				{
					letter = "s";
					if (sequence(3) == 1)
					{
						letter = "h";
						if (sequence(4) == 1)
						{
							letter = "5";
						}
						else if (sequence(4) == 2)
						{
							letter = "4";
						}
					}
					else if (sequence(3) == 2)
					{
						letter = "v";
						if (sequence(4) == 2)		
						{
							letter = "3";
						}				
					}
				}
				else if (sequence(2) == 2)
				{
					letter = "u";
					if (sequence(3) == 1)
					{
						letter = "f";
					}
					else if (sequence(3) == 2)
					{
						if (sequence(4) == 2)
						{
							letter = "2"
						}
					}
				}
			}
			if else (sequence(1) == 2)
			{
				letter = "a";
				if (sequence(2) == 1)
				{
					letter = "r";
					if (sequence(3) == 1)
					{
						letter = "l";
					}
				}
				else if (sequence(2) == 2)
				{
					letter = "w";
					if (sequence(3) == 1)
					{
						letter = "p";
					}
					else if (sequence(3) == 2)
					{
						letter = "j";
						if (sequence(4) == 2)
						{
							letter = "1";
						}
					}
				}
			}
			
		}
		else if (sequence(0) == 2)
		{
			letter = "t";
			if (sequence(1) == 1)
			{
				letter = "n";
				if (sequence(2) == 1)
				{
					letter = "d";
					if (sequence(3) == 1)
					{
						letter = "b";
						if (sequence(4) == 1)
						{
							letter = "6";
						}
					}
					else if (sequence(3) == 2)
					{
						letter = "x";
					}
				}
				else if (sequence(2) == 2)
				{
					letter = "k";
					if (sequence(3) == 1)
					{
						letter = "c";
					}
					else if (sequence(3) == 2)
					{
						letter = "y";
					}
				}				
			}
			else if (sequence(1) == 2)
			{	
				letter = "m";
				if (sequence(2) == 1)
				{
					letter = "g";
					if (sequence(3) == 1)
					{
						letter = "z";
						if (sequence(4) == 1)
						{
							letter = "7";
						}
					}
					else if (sequence(3) == 2)
					{
						letter = "q";
					}
				}
				else if (sequence(2) == 2)
				{
					letter = "o";
					if (sequence(3) == 1)
					{
						if (sequence(4) == 1)
						{
							letter = "8";
						}
					}
					else if (sequence(3) == 2)
					{
						if (sequence(4) == 1)
						{
							letter = "9";
						}
						else if (sequence(4) == 2)
						{
							letter = "0";
						}
					}
				}
			}
		}
		/*end of morse code section*/
		
		//clear the counter
		TCCR1B |= (1<<WGM12);
				
	}
	TCCR1B ^= (1<<ICES1);
}

