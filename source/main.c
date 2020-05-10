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
#include <avr/interrupt.h>
#include <stdlib.h>	
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif	

enum States { Start, Begin, Increase, Decrease, WaitInc, WaitDec, Reset }state;
unsigned char tempB = 0x00;
unsigned char tempA = 0x00;
int increaseTimer = 0;
int decreaseTimer = 0;

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
		increaseTimer = 0;
		state = WaitInc;
		break;
	case Decrease:
		decreaseTimer = 0;
		state = WaitDec;
		break;
	case WaitInc:
		if (increaseTimer >= 10)
		{
			state = Increase;
			break;
		}
		else if (tempA == 0x01)
		{
			increaseTimer++;
			state = WaitInc;
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
	case WaitDec:
		if (decreaseTimer >= 10)
		{
			state = Decrease;
			break;
		}
		else if (tempA == 0x02)
		{
			decreaseTimer++;
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
		if (tempB >= 0x09)
		{
			tempB = 0x09;
			break;
		}
		else
		{
			tempB = tempB + 0x01;
			break;
		}
	}
	case Decrease:
	{
		if (tempB <= 0x00)
		{
			tempB = 0x00;
			break;
		}
		else
		{
			tempB = tempB - 0x01;
			break;
		}
	}
	case Reset:
	{
		tempB = 0x00;
		break;
	}
	default:
		break;
	}

	PORTB = tempB;
}

int main(void)
{
	state = Start;
	tempB = 0x00;
	DDRA = 0x00;	PORTA = 0xFF;
	DDRB = 0xFF;	PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	while (1)
	{
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}