/*
 * Serial.h
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <sys/types.h>

class Serial {

private:
	// arranged alphabetical order
	volatile int * base; //32 bit base. uint32_t couldn't be found?
	int baud_date;
	short data_bits;
	int fd;
	char * name;
	char parity;
	int stop_bits;

	int write(char * message, size_t len);
	int write_en();
	int read_en();
	int toggle_mode();

public:
	Serial();
	virtual ~Serial();

	int write(char * message);
	int read(char * message);
};

#endif /* SERIAL_H_ */
