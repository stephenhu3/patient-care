/*
 * PatientVGA.c
 *
 *  Created on: Dec 2, 2015
 *      Author: aip
 */

#include "assert.h"

#include "PatientVGA.h"

#define MARGIN 10
#define WIDTH 80
#define HEIGHT 60
#define HOR_SPACE 80
#define VER_SPACE 60
#define TOT_SPACE 480

void vga_print_margins(alt_up_char_buffer_dev * vga, const char * message, size_t len)
{
	assert(vga);
	alt_up_char_buffer_clear(vga);
	if(!message) return; // no point printing nothing
	else alt_up_char_buffer_string(vga, message, MARGIN, MARGIN);
//	int increment = 0;
//	int line = 0;
//	if(len > TOT_SPACE) len = TOT_SPACE; // we're gonna overflow!!
//	if(len > HOR_SPACE ) // takes more than a line
//	{ // will go to next line
//		for(int i = 0; i < len; i += increment) // we're doing by line
//		{
//			if(len-i > HOR_SPACE) // there's more than one line left
//			{
//				int last_space_index = HOR_SPACE; // start from the back
//				while(message[i+last_space_index--] != ' ');
//			}
//		}
//	}
}
