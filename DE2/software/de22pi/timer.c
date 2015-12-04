/*
 * timer.c
 *
 *  Created on: Nov 28, 2015
 *      Author: aip
 */

#include <stdio.h>
#include "altera_avalon_timer_regs.h"
#include "system.h"
#include "alt_types.h"
#include "timer.h"
#include "sys/alt_irq.h"

#define DEBUG 0
#define MAX_TICKS 0xFFFFFFFF

/*
 * Initialize the timer IRQ given a base address, and register the function
 * listed as the handler when IRQ is HI.
 *
 * Due to limitations of 32 bit, the longest period possible with this device
 * is 858.9934592
 */
void init_timer_irq(alt_u32 base, alt_u32 irq_id, void * function, void * context, double secs) {
	alt_u32 numticks = MAX_TICKS;
	IOWR_ALTERA_AVALON_TIMER_STATUS(base, 0x0);
	if(secs < 0) {
		secs = -secs;
	}
	if(secs * TIMER_0_FREQ < MAX_TICKS) {
		numticks = secs * TIMER_0_FREQ;
	}

	IOWR_ALTERA_AVALON_TIMER_PERIODH(base, numticks >> 16);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(base, numticks & ALTERA_AVALON_TIMER_PERIODL_MSK);

//	alt_irq_context timer_context = alt_irq_disable_all();
	alt_irq_register(TIMER_0_IRQ, context, function);
//	alt_irq_enable_all(timer_context);

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
	IOWR_ALTERA_AVALON_TIMER_CONTROL(base,
			ALTERA_AVALON_TIMER_CONTROL_ITO_MSK |
			ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
			ALTERA_AVALON_TIMER_CONTROL_START_MSK
			);


}

int get_snapshot(alt_u32 base) {
	IOWR_ALTERA_AVALON_TIMER_SNAPL(base, 0x0); // trigger the snapshot to be taken
	int snap_hi = IORD_ALTERA_AVALON_TIMER_SNAPH(base) << 16;
	int snap_lo = IORD_ALTERA_AVALON_TIMER_SNAPL(base);
	return snap_hi + snap_lo;
}
