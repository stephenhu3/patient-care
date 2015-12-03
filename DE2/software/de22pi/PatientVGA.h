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
void vga_print_margins(alt_up_char_buffer_dev* vga, const char * message, size_t len);

#endif /* PATIENTVGA_H_ */
