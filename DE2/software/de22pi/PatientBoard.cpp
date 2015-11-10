/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "PatientBoard.h"
// #include sys.h

// ISR to check if the most recently written character is null (end string)
void check_write_ISR()
{
	// TODO: implement
}

void push_pin_ISR()
{
	// TODO: implement
}

// once null has been reached, read string up until that point to process
// the string, and then clear the memory? increment the counter?
void process_string()
{
	// TODO: implement
}

// write the message to the shared memory so that the PI can read the response
void write_message()
{
	// TODO: implement
}

// sets the right pins the right values. Low level function used to info hide
void set_pins()
{
	// TODO: implement
}



int main()
{
	// initialize the ISR
	// when display needs to be updated, do so (see string that's sent)
	// use timer to figure out how long we'll turn off the alarm
	//
  return 0;
}
