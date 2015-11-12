/*
 * Serial.h
 *
 *  Created on: Nov 9, 2015
 *      Author: aip
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#define WRITE_MASK 0x0020
#define WRITE_EN 0x0002
#define READ_MASK 0x0010
#define READ_EN 0x0001

class Serial {

private:
	// arranged alphabetical order
	volatile alt_u32 * base_reg;
	volatile alt_u32 * ctrl_reg;
	volatile int edge_capture;
	void * edge_capture_ptr;
	int baud_date;
	unsigned long int irq_id;
	short data_bits;
	int fd; // do i even need this?
	char * name;
	char parity;
	int stop_bits;
	char write_message[256];
	char read_message[256];
	bool write_mode;

	int write(char * message, int num_bytes);
	int write_byte(char * byte);
	int write_en();
	int read_en();
	int toggle_mode();
	int init_irq();

public:
	Serial();
	virtual ~Serial();

	int write(char * message);
	int read(char * message);

	void * process_Serial(void * context, unsigned long int id);
};

#endif /* SERIAL_H_ */
