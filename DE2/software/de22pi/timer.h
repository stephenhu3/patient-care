/*
 * timer.h
 *
 *  Created on: Nov 28, 2015
 *      Author: aip
 */

#ifndef TIMER_H_
#define TIMER_H_

struct timer;
typedef struct timer timer;


void init_timer_irq(alt_u32 base, alt_u32 irq_id, void * function, unsigned int secs);

#endif /* TIMER_H_ */
