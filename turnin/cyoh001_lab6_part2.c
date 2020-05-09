/*	Author: cyoh001
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - Increaselude for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdlib.h>	
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

enum States { Start, Begin, Increase, Decrease, Wait, Reset }state;
unsigned char tempC = 0x00;
unsigned char tempA = 0x00;

void Tick() {
	tempA = ~PINA;
	switch (state) { //Transitions
	case Start:
	{
		state = Begin;
		break;
	}
	case Begin:
		if (tempA == 0x01)
		{
			state = Increase;
			break;
		}
		else if (tempA == 0x02)
		{
			state = Decrease;
			break;
		}
		else if (tempA == 0x03)
		{
			state = Reset;
			break;
		}
		else
		{
			state = Begin;
			break;
		}
	case Increase:
		state = Wait;
		break;
	case Decrease:
		state = Wait;
		break;
	case Wait:
		if ((tempA == 0x01) || (tempA == 0x02))
		{
			state = Wait;
			break;
		}
		else if (tempA == 0x03)
		{
			state = Reset;
			break;
		}
		else
		{
			state = Begin;
			break;
		}
	case Reset:
		if ((tempA == 0x01) || (tempA == 0x02))
		{
			state = Reset; break;
		}
		else
		{
			state = Begin; break;
		}
	default:
		state = Start;
		break;
	}

	switch (state) { //State actions
	case Begin:
		break;
	case Increase:
	{
		if (tempC >= 0x09)
		{
			tempC = 0x09;
			break;
		}
		else
		{
			tempC = tempC + 0x01;
			break;
		}
	}
	case Decrease:
	{
		if (tempC <= 0x00)
		{
			tempC = 0x00;
			break;
		}
		else
		{
			tempC = tempC - 0x01;
			break;
		}
	}
	case Wait:
		break;
	case Reset:
	{
		tempC = 0x00;
		break;
	}
	default:
		break;
	}

	PORTC = tempC;
}

int main(void)
{
	state = Start;
	tempC = 0x00;
	DDRA = 0x00;	PORTA = 0xFF;
	DDRC = 0xFF;	PORTC = 0x00;
	while (1)
	{
		Tick();
	}
	return 0;
}