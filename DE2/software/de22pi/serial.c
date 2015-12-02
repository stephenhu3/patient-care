/*
 * Serial.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "serial.h"
#include "system.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"

#define DEBUG 0
#define TIMEOUT "&"

#define ACKNOWLEDGE "Acknowledged."

serial * initialize_serial(alt_u32 base, const char * name,
		int IRQ_num, int IRQ_ctrlr_ID) {
	serial * pi_serial = malloc(sizeof(pi_serial));
	pi_serial->fd = open(name, O_NONBLOCK | O_RDWR);
	if(pi_serial->fd >= 0)
	{
		pi_serial->base = base;
		pi_serial->IRQ_num = IRQ_num;
		pi_serial->IRQ_ctrlr_ID = IRQ_ctrlr_ID;
	}
	return pi_serial;
}

int serial_write(serial * serial, char * message) {
	return write(serial->fd, message, strlen(message));
}

void * serial_read(void * context, alt_u32 id) {
	alt_irq_context cpu_sr;
	cpu_sr = alt_irq_disable_all();
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0x0);
	serial * de22pi_serial = (serial *) context;
	if (de22pi_serial->fd >=0) // file exists
	{
		char temp_msg[MSG_LEN];
		int num_read;
		num_read = read(de22pi_serial->fd, (void *) temp_msg, MSG_LEN);
		temp_msg[num_read++] = '\0';
		if(num_read > 0)
		{
			printf("I read something.\n");
			de22pi_serial->msg_read = 1;
			if(strcmp(temp_msg,TIMEOUT) == 0) {
				de22pi_serial->timeout = 1;
			}
			de22pi_serial->msg_index = num_read;
			strncpy(de22pi_serial->read_message, temp_msg, num_read);
			if(DEBUG)
				write(de22pi_serial->fd, de22pi_serial->read_message, de22pi_serial->msg_index);
			printf("%s\n", de22pi_serial->read_message);
			// write(de22pi_serial->fd, ACKNOWLEDGE, strlen(ACKNOWLEDGE));
		}
		alt_irq_enable_all(cpu_sr);
		return (void *) de22pi_serial;
	}
	else
	{// things didn't work
		printf("File descriptor less than 0.\n");
		alt_irq_enable_all(cpu_sr);
		return context;
	}
}
