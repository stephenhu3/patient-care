/*
 * PatientVGA.c
 *
 *  Created on: Dec 2, 2015
 *      Author: aip
 */

#include "assert.h"
#include <string.h>
#include <stdio.h>
#include "io.h"

#include "PatientVGA.h"

#define CHAR_PIXEL_RATIO 4

#define CHAR_WIDTH 80
#define CHAR_HEIGHT 60

#define INSTR_HOR_MARGIN 20
#define INSTR_VER_MARGIN 20
#define INSTR_HOR_SPACE 40
#define INSTR_VER_SPACE 20

#define MED_HOR_MARGIN INSTR_HOR_MARGIN
#define MED_VER_MARGIN INSTR_VER_MARGIN-5
#define MED_HOR_SPACE INSTR_HOR_SPACE
#define MED_VER_SPACE 3

#define PIX_WIDTH 320
#define PIX_HEIGHT 240

#define INSTR_X1 (INSTR_HOR_MARGIN-1)*CHAR_PIXEL_RATIO
#define INSTR_X2 (INSTR_HOR_MARGIN+INSTR_HOR_SPACE+1)*CHAR_PIXEL_RATIO
#define INSTR_Y1 (INSTR_VER_MARGIN-1)*CHAR_PIXEL_RATIO
#define INSTR_Y2 (INSTR_VER_MARGIN+INSTR_VER_SPACE+1)*CHAR_PIXEL_RATIO

#define MED_X1 (MED_HOR_MARGIN-1)*CHAR_PIXEL_RATIO
#define MED_X2 (MED_HOR_MARGIN+INSTR_HOR_SPACE+1)*CHAR_PIXEL_RATIO
#define MED_Y1 (MED_VER_MARGIN-1)*CHAR_PIXEL_RATIO
#define MED_Y2 (MED_VER_MARGIN+MED_VER_SPACE+1)*CHAR_PIXEL_RATIO

#define RED 0x5
#define GREEN 0x1F
#define BLUE 0x1C

void vga_print_instr(alt_up_char_buffer_dev * char_buffer,
		volatile int * pixel_drawer_base, const char * instruct, size_t len)
{
	vga_string(char_buffer, instruct, len, INSTR_HOR_MARGIN, INSTR_HOR_SPACE, INSTR_VER_MARGIN, INSTR_VER_SPACE);
	vga_box(pixel_drawer_base, INSTR_X1, INSTR_X2, INSTR_Y1, INSTR_Y2, RED, GREEN, BLUE);
}

void vga_print_meds(alt_up_char_buffer_dev* char_buffer,
		volatile int * pixel_drawer_base, const char * med_name, size_t len) {
	vga_string(char_buffer, med_name, len, MED_HOR_MARGIN, MED_HOR_SPACE, MED_VER_MARGIN, MED_VER_SPACE);
	vga_box(pixel_drawer_base, INSTR_X1, INSTR_X2, INSTR_Y1, INSTR_Y2, RED, GREEN, BLUE);
}

void vga_string(alt_up_char_buffer_dev* char_buffer,
		const char * message, size_t len,
		unsigned int hor_margin, unsigned int hor_space,
		unsigned int ver_margin, unsigned int ver_space) {
	assert(char_buffer);
	alt_up_char_buffer_clear(char_buffer);
	if(!message) return; // no point printing nothing
	int increment = 0;
	int line = 0;
	if(len > hor_space * ver_space) len = hor_space*ver_space; // we're gonna overflow!!
	int i = 0;
	for(i = 0; i < len; i += increment) // we're doing by line
	{
		if(len-i > hor_space) // there's more than one line left
		{
			int last_space_index = hor_space; // start from the back
			// look for the last space within the range
			while(message[--last_space_index+i] != ' ' && last_space_index >= 0);
			if(last_space_index == 0) increment = hor_space; // never found space. just print all of it
			else increment = last_space_index + 1; // possibility that last_space_index == 1;
			char toprint[increment];
			strncpy(toprint, message+i, increment-1);
			toprint[increment-1] = '\0';
			alt_up_char_buffer_string(char_buffer, toprint, hor_margin, ver_margin+line);
			line++;
		}
		else {
			char toprint[len-i];
			strncpy(toprint, message+i, len-i);
			toprint[len-i] = '\0';
			alt_up_char_buffer_string(char_buffer, message+i, hor_margin, ver_margin+line);
			increment = len-i;
		}
	}
}

// INVARIANT: red and blue are 5 bit, green is 6 bit
void vga_box(volatile int * pixel_drawer_base,
		unsigned int x1, unsigned int x2,
		unsigned int y1, unsigned int y2,
		alt_u8 red, alt_u8 green, alt_u8 blue) {
	alt_u32 color_code = red << 11 | green << 5 | blue;
	IOWR_32DIRECT(pixel_drawer_base, 0, x1);
	IOWR_32DIRECT(pixel_drawer_base, 4, y1);
	IOWR_32DIRECT(pixel_drawer_base, 8, x2);
	IOWR_32DIRECT(pixel_drawer_base,12, y2);
	IOWR_32DIRECT(pixel_drawer_base,16, color_code);
	IOWR_32DIRECT(pixel_drawer_base,20, 1);
	while(IORD_32DIRECT(pixel_drawer_base, 20) == 0);
}
