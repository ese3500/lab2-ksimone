/*
 * 350Lab2.c
 *
 * Created: 2/6/2023 6:04:21 PM
 * Author : simon
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void Initialize();

int main(void)
{
	Initialize();
    while (1) 
    {							
    }
}

void Initialize()
{
	TIMSK1 |= (1<<ICIE1);
	PCICR |= (1<<PCIE2);
	TCCR1B |= (1<<ICES1);
	DDRB |= (1<<DDB5);
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	TIFR1 |= (1<<ICF1);
	if (PINB & (1<<PINB0))
	{
		PORTB |= (1<<PORTB5);
	}
	else
	{
		PORTB &= ~(1<<PORTB5);
	}
	TCCR1B ^= (1<<ICES1);
}

