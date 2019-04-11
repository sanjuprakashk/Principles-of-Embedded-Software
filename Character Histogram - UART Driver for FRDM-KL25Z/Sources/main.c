/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "circular_buffer.h"

#ifndef BB

#include <MKL25Z4.h>

#define CNT 100000

#define INTERRUPTS 1

c_buff *RX_buff = NULL;
c_buff *TX_buff = NULL;

uint8_t rx_data = 0;
uint8_t tx_data = 0;

bool rx_flag = 0;
bool tx_flag = 0;

bool polling_select = 0;
bool interrupt_select = 0;

int disable_toggle = 0;
int count[256] = {0};
char option ='0';

void led_init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTB_PCR19 = PORT_PCR_MUX(1);

	GPIOB_PSOR |= (1 << 19); // Set as output

	GPIOB_PDDR |= (1 << 19);
}



void uart_config()
{
	uint16_t Baud;
	SIM_SCGC5 |= SIM_SCGC5_PORTA(1); // Enable port A as UART0 is part of port A
	SIM_SCGC4 |= SIM_SCGC4_UART0(1); // Enable UART0 through system clock gating register

	//SIM_SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);
	//SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(1);
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // Activates the UART0 clock
	PORTA_PCR1 |= PORT_PCR_MUX(2); // Enable alternative functionality 2 for allowing UART0 on PTA1
	PORTA_PCR2 |= PORT_PCR_MUX(2); // Enable alternative functionality 2 for allowing UART0 on PTA2
	UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Disable Transmitter and Receiver

	UART0_S1 = 0;
	UART0_C1 = 0;
	UART0_C4 |= (0x0F & UART0_C4_OSR_MASK);

	//Baud = (uint16_t)(SystemCoreClock/(BAUD_RATE * 16)); // Setting Baud to 57600
	Baud = 12;

	UART0_BDH &= ~(UART0_BDH_SBR_MASK);
	UART0_BDH |= UART_BDH_SBR((Baud>>8));
	UART0_BDL &= ~(UART0_BDL_SBR_MASK);
	UART0_BDL |= UART_BDH_SBR(Baud);

	UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Enable Transmitter and Receiver

if (interrupt_select)
{
	NVIC_EnableIRQ(UART0_IRQn);
	UART0_C2 |= UART0_C2_RIE_MASK;
}
}

void uart_interrupt_config()
{
	NVIC_EnableIRQ(UART0_IRQn);
	UART0_C2 |= UART0_C2_RIE_MASK;
}

char uart_transmit()
{
	while(!(UART0->S1 & UART_S1_RDRF_MASK));

	return UART0_D;
}

void uart_receive(char tx_data)
{
	while(!(UART0_S1 & UART_S1_TDRE_MASK) && !(UART0_S1 & UART_S1_TC_MASK));

	UART0_D = (tx_data);
}

/* Code to print string */
void dispStr(char *data)
{
	while(*data)
	{
		uart_receive(*data);
		data++;
	}
}

void menu()
{
	dispStr("Welcome to Project 2 of PES \r\n");
	dispStr("Menu: \r\n");
	dispStr("1. Polling\r\n");
	dispStr("2. Interrupt\r\n");
	dispStr("3. Exit\r\n");
	while(option == '0')
	{
		option = uart_transmit();
		uart_receive(option);
	}
	if(option == '1')
	{
		dispStr("Polling selected\r\n");
		polling_select = 1;
		interrupt_select = 0;
	}
	else if(option == '2')
	{
		dispStr("Interrupt selected\r\n");
		interrupt_select = 1;
		polling_select = 0;
	}
}

void UART0_IRQHandler()
{
  if (((UART0_S1) & UART0_S1_RDRF_MASK))
  {
    cb_add_data(RX_buff, UART0_D);
    cb_remove_data(RX_buff, &rx_data);
    count[rx_data]++;
    rx_flag = 1;
	//generate_report();
  }
}

int AP = 0;
int diff = 5;
void print_series()
{
	char msg[10] = {0};
	AP = AP + diff;
	sprintf(msg, "%d\t", AP);
	dispStr(msg);
}
void generate_report()
{
	dispStr("\r\n");
	int i = 0;
	char msg[16] = {0};

	while (i <= 256)
	{
		if (count[i] != 0)
		{
			sprintf(msg, "%c is %d\r\n", i, count[i]);
			dispStr(msg);
		}
		i++;
	}
	tx_flag = 0;
	rx_flag = 0;
}
#endif

int main(void)
{

#ifndef BB
	RX_buff = (c_buff *)malloc(sizeof(c_buff));
	//TX_buff = (c_buff *)malloc(sizeof(c_buff));

	cb_new(RX_buff, 50);
	//cb_new(TX_buff, 256);

	uart_config();
	led_init();
	menu();
	int counter = 0;
	char data;

	int blink_count = 0;

	if(interrupt_select)
	{
		dispStr("Entering interrupt mode\r\n");
		uart_interrupt_config();
		while(1)
		{
			//int i = 0;
			counter++;
			if (rx_flag)
			{
				blink_count = 0;
				if(counter >= CNT)
					{
						counter = 0;
						disable_toggle = 1;
						generate_report();
						GPIOB->PSOR = (1<<19);
						disable_toggle = 0;
					}
				}
			if(!disable_toggle)
				blink_count++;
			if(blink_count >=100000)
			{
				blink_count = 0;
				//GREEN_TOGGLE;
				GPIOB->PTOR |= (1<<19);
				print_series();
			}
		}
	}

	else if (polling_select)
	{
		dispStr("Entering polling mode\r\n");
		while(1)
	    {
	    	data = uart_transmit();
	    	uart_receive(data);
			//asciiVals(data);

	    }
	}


		return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
