/*
 * 350Lab2.c
 *
 * Created: 2/6/2023 6:04:21 PM
 * Author : simon
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 62500
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "inc/uart.h"

/*--------------------Variables---------------------------*/
char String[25];
int pulse = 0; //to store the value of the last button press, ./-
int spaced = 0; //to determine whether a letter has just been printed
int sequence[5]; //to hold up to five dots/dashes at a time
char letter; //to hold the value to be printed


void Initialize();

int main(void)
{	
	Initialize();
    while (1) 
    {		
		//testing timer
		
		//sprintf(String, "%u \n", TCNT1);
		//UART_putstring(String);
		
		//if timer gets to 25000, record a space if necessary
		if (spaced == 0) //if we have not yet printed the current letter being typed
		{
			//if timer gets to 25000
			//record letter
			
			if (TCNT1 == 25000) // if we get to 400ms without a new button press
			{
			
				// print timer, should be 25000
				sprintf(String, "%u \n", TCNT1);
				UART_putstring(String);

			
				// start of morse code section, decoding whatever is stored in sequence
				if (sequence[0] == 1)
				{
					letter = 'a'; // .
					if  (sequence[1] == 1)
					{
						letter = 'i'; // ..
						if (sequence[2] == 1)
						{
							letter = 's'; // ...
							if (sequence[3] == 1)
							{
								letter = 'h'; // ....
								if (sequence[4] == 1)
								{
									letter = '5'; // .....
								}
								else if (sequence[4] == 2)
								{
									letter = '4'; // ....-
								}
							}
							else if (sequence[3] == 2)
							{
								letter = 'v'; // ...-
								if (sequence[4] == 2)
								{
									letter = '3'; // ...--
								}
							}
						}
						else if (sequence[2] == 2)
						{
							letter = 'u'; // ..-
							if (sequence[3] == 1)
							{
								letter = 'f'; // ..-.
							}
							else if (sequence[3] == 2)
							{
								if (sequence[4] == 2)
								{
									letter = '2'; // ..---
								}
							}
						}
					}
					else if (sequence[1] == 2)
					{
						letter = 'a'; // .-
						if (sequence[2] == 1)
						{
							letter = 'r'; // .-.
							if (sequence[3] == 1)
							{
								letter = 'l'; // .-..
							}
						}
						else if (sequence[2] == 2)
						{
							letter = 'w'; // .--
							if (sequence[3] == 1)
							{
								letter = 'p'; // .--.
							}
							else if (sequence[3] == 2)
							{
								letter = 'j'; // .---
								if (sequence[4] == 2)
								{
									letter = '1'; // .----
								}
							}
						}
					}
				
				}
				else if (sequence[0] == 2)
				{
					letter = 't'; // -
					if (sequence[1] == 1)
					{
						letter = 'n'; // -.
						if (sequence[2] == 1)
						{
							letter = 'd'; // -..
							if (sequence[3] == 1)
							{
								letter = 'b'; // -...
								if (sequence[4] == 1)
								{
									letter = '6'; // -....
								}
							}
							else if (sequence[3] == 2)
							{
								letter = 'x'; // -..-
							}
						}
						else if (sequence[2] == 2)
						{
							letter = 'k'; // -.-
							if (sequence[3] == 1)
							{
								letter = 'c'; // -.-.
							}
							else if (sequence[3] == 2)
							{
								letter = 'y'; // -.--
							}
						}
					}
					else if (sequence[1] == 2)
					{
						letter = 'm'; // --
						if (sequence[2] == 1)
						{
							letter = 'g'; // --.
							if (sequence[3] == 1)
							{
								letter = 'z'; // --..
								if (sequence[4] == 1)
								{
									letter = '7'; // --...
								}
							}
							else if (sequence[3] == 2)
							{
								letter = 'q'; // --.-
							}
						}
						else if (sequence[2] == 2)
						{
							letter = 'o'; // ---
							if (sequence[3] == 1)
							{
								if (sequence[4] == 1)
								{
									letter = '8'; // ---..
								}
							}
							else if (sequence[3] == 2)
							{
								if (sequence[4] == 1)
								{
									letter = '9'; // ----.
								}
								else if (sequence[4] == 2)
								{
									letter = '0'; // -----
								}
							}
						}
					}
				}
				//end of morse code section			
			
			
				//print the decoded letter and then a space
				sprintf(String, "%c \n", letter);
				UART_putstring(String);
			
				spaced = 1; //don't print anything else until a new button press
				
				//clearing the sequence array
				sequence[0] = 0;
				sequence[1] = 0;
				sequence[2] = 0;
				sequence[3] = 0;
				sequence[4] = 0;
				
				TCCR1B &= ~(1<<TCNT1); //clear the counter
			}
		}
    }
}

void Initialize()
{
	cli(); // clear global interrupts
	
	UART_init(BAUD_PRESCALER);  // set up uart comms rate
	
	TIMSK1 |= (1<<ICIE1); // enable timer interrupts (check this)
	PCICR |= (1<<PCIE2); // probably not necessary
	DDRB |= (1<<DDB5); // set up pin 5 as input
	TCCR1B |= (1<<ICES1); // Input Capture Edge Select = rising edge
	TCCR1B |= (1<<ICF1);
	
	//may need to adjust the frequencies since they might be overflowing before they reach their maximum value
	//CLKPR = (1 << CLKPCE); // Enable change to clock prescaler
	//CLKPR = (1 << CLKPS3); // Divide by 256
	
	// set timer prescaler (just changed from 1024 to 256, check if this  worked
	TCCR1B &= ~(1<<CS10); 
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	// set timer to normal mode
	TCCR1B &= ~(1<<WGM10);
	TCCR1B &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	
	TIFR1 |= (1<<ICF1); // clear interrupt flag
	
	if (PINB & (1<<PINB0)) //check if this is rising edge (button press)
	{
		//turn on board LED on
		PORTB |= (1<<PORTB5);
		
		//test serial monitor
		//sprintf(String, "testing \n");
		//UART_putstring(String);
		
		//set counter to 0
		TCCR1B &= ~(1<<TCNT1);
		
		//test
		//sprintf(String, "testing press %u \n", ICR1);
		//UART_putstring(String);
		
		spaced = 0; //enable printing again
		
	}
	else // code for falling edge (button release)
	{
		//turn on board LED off
		PORTB &= ~(1<<PORTB5);
		
		//record pulse
		pulse = TCNT1;
		
		//test
		//sprintf(String, "testing release %u \n", ICR1);
		//UART_putstring(String);
		
		// set pulse to the correct value based on length of button press
		if ((pulse > 1875) && (pulse < 12500))
		{
			pulse = 1; // correlates to .
		}
		if ((pulse > 12500) && (pulse < 25000))
		{
			pulse = 2; // correlates to -
		}
		
		// save pulse in sequence array
		if ((pulse == 1) || (pulse == 2))
		{
			if (sequence[0] == 0)
			{
				sequence[0] = pulse;
				sprintf(String, "0 = %c \n", pulse);
				UART_putstring(String);
			}
			else if (sequence[1] == 0)
			{
				sequence[1] = pulse;
				sprintf(String, "0 = %c \n", pulse);
				UART_putstring(String);
			}
			else if (sequence[2] == 0)
			{
				sequence[2] = pulse;
				sprintf(String, "0 = %c \n", pulse);
				UART_putstring(String);
			}
			else if (sequence[3] == 0)
			{
				sequence[3] = pulse;
				sprintf(String, "0 = %c \n", pulse);
				UART_putstring(String);
			}
			else if (sequence[4] == 0)
			{
				sequence[4] = pulse;
				sprintf(String, "0 = %c \n", pulse);
				UART_putstring(String);
			}
		}
		
		//clear the counter
		TCCR1B &= ~(1<<TCNT1);
				
	}
	TCCR1B ^= (1<<ICES1);
}
