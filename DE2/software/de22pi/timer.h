/*
 * timer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: aip
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "alt_types.h"

void init_timer_irq(alt_u32 base, alt_u32 irq_id, void * function,
		void * context, double secs);
int get_snapshot(alt_u32 base);

#endif /* TIMER_H_ */
