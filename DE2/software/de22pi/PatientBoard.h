/*
 * de22pi.h
 *
 *  Created on: Nov 6, 2015
 *      Author: aip
 */

#ifndef PATIENTBOARD_H_
#define PATIENTBOARD_H_

#include "alt_types.h"

void init_PUSH_IRQ();
void push_ISR(void * context, alt_u32 id);



#endif /* PATIENTBOARD_H_ */
