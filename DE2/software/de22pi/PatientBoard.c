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
#include <string.h>
#include <unistd.h>

#include "assert.h"

// altera includes
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
//#include "altera_up_avalon_audio.h"
//#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_character_lcd.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "sys/alt_irq.h"
#include "system.h"

// project files
#include "serial.h"
#include "timer.h"
#include "PatientBoard.h"
#include "PatientVGA.h"

#define DEBUG 0

#define BLANK_LINE "                " // something is here don't touch!
#define BASE_DEMO_DIR 0xFFFFFFFC // most pins don't matter but last eight need to be
#define KEY_0_MASK 0x01
#define KEYS (volatile char *) KEYS_BASE
#define WAIT_ACK 0
#define WAIT_PI 1
#define WAIT_PUSH 2
#define PERIOD_MINUTES 1
#define PERIOD_SECONDS PERIOD_MINUTES * 60
#define PUSH_DONE 3
#define PUSH_MSG "K"

int state = WAIT_PI;
int state_changed = 1;
int button_pushed = 0;

void push_isr(void * context, alt_u32 id)
{
	alt_irq_context cpu_sr = alt_irq_disable_all();
	int push_val = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	if(push_val & KEY_0_MASK) {
		button_pushed=1;
	}
	alt_irq_enable_all(cpu_sr);
	alt_irq_disable(KEYS_IRQ);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x0);
}

void char_lcd_clear( alt_up_character_lcd_dev* de2_lcd ) {
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
	alt_up_character_lcd_string(de2_lcd, BLANK_LINE);
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 1);
	alt_up_character_lcd_string(de2_lcd, BLANK_LINE);
}

void init_push_irq(void * function) {
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x0);
	alt_irq_register((alt_u32)KEYS_IRQ, NULL, function);
	alt_irq_disable(KEYS_IRQ);
}

void clear_all(alt_up_char_buffer_dev *char_buffer,
		alt_up_pixel_buffer_dma_dev* pixel_buffer,
		alt_up_character_lcd_dev* de2_lcd) {
	alt_up_char_buffer_clear(char_buffer);
	char_lcd_clear(de2_lcd);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
}

int main()
{
	// setup
	alt_up_char_buffer_dev *char_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");
	assert(char_buffer);

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(VIDEO_PIXEL_BUFFER_DMA_0_NAME);
	assert(pixel_buffer);

	alt_up_character_lcd_dev* de2_lcd = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
	assert(de2_lcd);
	alt_up_character_lcd_init(de2_lcd);

	serial * de22pi_rs232 = initialize_serial(RS232_0_0_BASE, RS232_0_0_NAME,
			RS232_0_0_IRQ, RS232_0_0_IRQ_INTERRUPT_CONTROLLER_ID);
	assert(de22pi_rs232);
	if(de22pi_rs232 == NULL) {
		printf("Serial initialisation for %s failed.\n", RS232_0_0_NAME);
		printf("Exiting...\n");
		return -1;
	}

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, SRAM_0_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_char_buffer_clear(char_buffer);

	init_timer_irq(TIMER_0_BASE, TIMER_0_IRQ, serial_read, (void *) de22pi_rs232, 10);
    init_push_irq(push_isr);

    // main section
    if(de22pi_rs232)
    {
		while (1)
		{
			if(button_pushed)
			{
				if(DEBUG)
					printf("%u\n", get_snapshot(TIMER_0_BASE)); // check pointer
				clear_all(char_buffer, pixel_buffer, de2_lcd);
				serial_write(de22pi_rs232, PUSH_MSG);
				button_pushed=0;
			}
			if(de22pi_rs232->timeout)
			{
				// clear VGA
				alt_irq_disable(KEYS_IRQ);
				clear_all(char_buffer, pixel_buffer, de2_lcd);
				de22pi_rs232->timeout = 0;
			}
			else if(de22pi_rs232->msg_read)
			{
				char * message;
				strcpy(message, de22pi_rs232->read_message);
				alt_up_char_buffer_clear(char_buffer);
				vga_print_instr(char_buffer,(volatile int *) PIXEL_DRAWER_0_BASE, message, strlen(message));
				char_lcd_clear(de2_lcd);
				alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
				alt_up_character_lcd_string(de2_lcd, message);
				de22pi_rs232->msg_read = 0;
				IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x0);
				alt_irq_enable(KEYS_IRQ);
			}
		}
    }
    return 0;
}
