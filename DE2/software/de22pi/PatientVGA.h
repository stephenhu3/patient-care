/*
 * PatientVGA.h
 *
 *  Created on: Dec 2, 2015
 *      Author: aip
 */

#ifndef PATIENTVGA_H_
#define PATIENTVGA_H_

#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "alt_types.h"

// given a VGA dev, will print text on screen with margins set in this file
void vga_print_instr(alt_up_char_buffer_dev* char_buffer,
		volatile int * pixel_drawer_base, const char * instruct, size_t len);
void vga_print_meds(alt_up_char_buffer_dev* char_buffer,
		volatile int * pixel_drawer_base, const char * med_name, size_t len);
void vga_string(alt_up_char_buffer_dev* char_buffer,
		const char * message, size_t len,
		unsigned int hor_margin, unsigned int hor_space,
		unsigned int ver_margin, unsigned int ver_space);
void vga_box(volatile int * pixel_drawer_base,
		unsigned int x1, unsigned int x2,
		unsigned int y1, unsigned int y2,
		alt_u8 red, alt_u8 green, alt_u8 blue);

#endif /* PATIENTVGA_H_ */
