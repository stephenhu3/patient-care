/*
 * timer.c
 *
 *  Created on: Nov 28, 2015
 *      Author: aip
 */

#include <stdio.h>
#include "altera_avalon_timer_regs.h"
#include "system.h"

#define DEBUG 1
#define MAX_TICKS

struct timer
{
	alt_u32 base;

};

/*
 * Initialize the timer IRQ given a base address, and register the function
 * listed as the handler when IRQ is HI.
 *
 * Due to limitations of 32 bit, the longest period possible with this device
 * is 858.9934592
 */
void init_timer_irq(alt_u32 base, alt_u32 irq_id, void * function, unsigned double secs) {
	// write to status register to clear Time-out (TO) bit
	// need to clear timeout bit by writing to status register when it goes HI
	IOWR_16DIRECT(base, ALTERA_AVALON_TIMER_STATUS_REG, 0x0);

	if( (secs * TIMER_0_FREQ) > MAX_TICKS) {

	}
	alt_u32 numticks = secs *
	/*
	 * set the timeout period in number of clock cycles
	 */
	//set the timer period

	/* set the control register bits:
	 * control[0] ITO = 1 enables IRQ generation once TO goes HI
	 * control[1] CONT = 1 sets the counter to restart once it hits zero
	 * control[2] START = 1 starts the counter - only works when register enabled
	 * control[3] STOP = 1 stops the counter - only works when register enabled
	 */
	IOWR_ALTERA_AVALON_TIMER_STATUS(base,
			ALTERA_AVALON_TIMER_CONTROL_ITO_MSK |
			ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
			ALTERA_AVALON_TIMER_CONTROL_START_MSK
			);
}
