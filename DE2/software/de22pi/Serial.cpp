/*
 * Serial.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#include <string.h>
#include "Serial.h"
#include "system.h"

Serial::Serial() {
	// TODO Auto-generated constructor stub
	this->baud_date = UART_0_BAUD;
	this->base = (volatile int *) UART_0_BASE;
	this->data_bits = UART_0_DATA_BITS;
	this->name = UART_0_NAME;
	this->parity = UART_0_PARITY;
	this->stop_bits = UART_0_STOP_BITS;
}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

int write(char * message) {
	// TODO: implement
	return write(message, strlen(message));
}

int write(char * message, size_t len) {
	// TODO: implement
	// check that there's space in the FIFO
	// write to the FIFO
	return -1;
}


