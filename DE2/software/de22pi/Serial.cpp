/*
 * Serial.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#include <string.h>
#include "Serial.h"
#include "system.h"
#include "sys/alt_irq.h"

Serial::Serial() {
	// TODO Auto-generated constructor stub
	this->baud_date = UART_0_BAUD;
	this->irq_id = UART_0_IRQ;
	this->base_reg = (volatile int *) UART_0_BASE;
	this->ctrl_reg = ((volatile int *) UART_0_BASE) +1;
	this->data_bits = UART_0_DATA_BITS;
	this->name = UART_0_NAME;
	this->parity = UART_0_PARITY;
	this->stop_bits = UART_0_STOP_BITS;
	this->edge_capture_ptr = (void*)&this->edge_capture;
	this->write_mode = false;
	init_irq();
}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

void * Serial::process_Serial(void * context, unsigned long int id) {
	if(this->ctrl_reg & READ_MASK) {

	}
	else if((this->ctrl_reg & WRITE_MASK) && write_mode) {

	}
	return context;
}

int Serial::init_irq() {
	this->ctrl_reg |= WRITE_EN | READ_EN;
	alt_irq_enable(this->irq_id);
	// return alt_irq_register( irq_id, edge_capture_ptr, process_Serial );
	return -1;
}

int Serial::write(char * message, int num_bytes) {
	// TODO: implement
	// check that there's space in the FIFO
	// write to the FIFO

	return -1;
}

int Serial::write(char * message) {
	// TODO: implement
	return write(message, (int) strlen(message));
}

