/*
 * Serial.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "system.h"
#include "sys/alt_irq.h"

#define DEBUG 1

struct serial {
	// arranged alphabetical order
	const char * name;
	int fd;
	alt_u32 base;
	alt_u32 IRQ_ctrlr_ID;
	alt_u32 IRQ_num;

	char write_message[256] = ""; // record of last message?
	char read_message[256] = "";
	int msg_index = 0;
	int msg_read = 0;
};

serial * initialize_serial(alt_u32 base, const char * name, alt_u32 IRQ_num,
		IRQ_ctrlr_ID) {
	serial * pi_serial;
	pi_serial->fd = open(name, O_NONBLOCK | O_RDWR);
	if(pi_serial->fd) {
		return NULL;
	}
	else
	{
		pi_serial->base = base;
		pi_serial->IRQ_num = IRQ_num;
		pi_serial->IRQ_ctrlr_ID = IRQ_ctrlr_ID;
	}
	return pi_serial;
}

int serial_write(serial * serial, char * message) {
	if(serial)
	// TODO: implement
	return -1;
}

void * serial_read(void * context, alt_u32 id) {
	serial * de22pi_serial = (serial *) context;
	char * later_gator = "See you later, alligator!\n";
	char * pushed = "PUSHED THE BUTTON!";
	if (de22pi_serial->fd) // file exists
	{
		if((de22pi_serial->msg_index = read(de22pi_serial, (void *) de22pi_serial->read_message, 1000)))
			de22pi_serial->msg_read = 1;
		if(DEBUG)
			write(de22pi_serial->fd, de22pi_serial->read_message, de22pi_serial->msg_index);
		return (void *) de22pi_serial;
	}
	else // things didn't work
		return context;
}
