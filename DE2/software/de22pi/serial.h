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
#define MSG_LEN 1024

typedef struct serial {
	// arranged alphabetical order
	const char * name;
	int fd;
	alt_u32 base;
	alt_u32 IRQ_ctrlr_ID;
	alt_u32 IRQ_num;

	int timeout;
	char write_message[MSG_LEN]; // record of last message?
	char read_message[MSG_LEN];
	int msg_index;
	int msg_read;
} serial;

serial * initialize_serial(alt_u32 base, const char * name, int IRQ_num, int IRQ_ctrlr_ID);
int serial_write(serial * serial, char * message);
void * serial_read(void * context, alt_u32 id);

#endif /* SERIAL_H_ */
