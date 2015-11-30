/*
 * Serial.h
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 *
 *      Class implementing the rs232 Interrupt service routines for CPEN391
 *      Patient-care project.
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "alt_types.h"

#define TIMER_MODE 0

struct serial;
typedef struct serial serial;

serial initialize_serial(FILE * serial, alt_u32 serial_IRQ_num);
int init_rs232_read_irq();
//void * process_serial(void * context, alt_u32 id);
void * serial_read(void * context, alt_u32 id);



#endif /* SERIAL_H_ */
