/*
 * de22pi.h
 *
 *  Created on: Nov 6, 2015
 *      Author: aip
 */

#ifndef PATIENTBOARD_H_
#define PATIENTBOARD_H_

#include "alt_types.h"

void init_push_irq();
void push_isr(void * context, alt_u32 id);
void init_timer_irq();


#endif /* PATIENTBOARD_H_ */
