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

#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer.h"
#include "altera_up_avalon_character_lcd.h"
#include "sys/alt_alarm.h"
#include "sys/alt_irq.h"
#include "system.h"

#include "PatientBoard.h"
#include "serial.h"

#define DEBUG 1

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
#define PUSH_MSG "Medicine taken"

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
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x0);
	alt_irq_enable_all(cpu_sr);
}

// write the message to the shared memory so that the PI can read the response
void write_message()
{
	// TODO: implement
}

int print_push_button(alt_up_character_lcd_dev* de2_lcd) {
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
	alt_up_character_lcd_string(de2_lcd, "PUSH BUTTON     ");
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 1);
	alt_up_character_lcd_string(de2_lcd, "PLEASE          ");
	return 0;
}

void char_lcd_clear( alt_up_character_lcd_dev* de2_lcd ) {
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
	alt_up_character_lcd_string(de2_lcd, BLANK_LINE);
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 1);
	alt_up_character_lcd_string(de2_lcd, BLANK_LINE);
}

void print_wait(alt_up_character_lcd_dev* de2_lcd) {
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
	alt_up_character_lcd_string(de2_lcd, "WAIT FOR        ");
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 1);
	alt_up_character_lcd_string(de2_lcd, "NEXT REQUEST!   ");
}

void print_wait_ack(alt_up_character_lcd_dev* de2_lcd) {
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 0);
	alt_up_character_lcd_string(de2_lcd, "AWAITING        ");
	alt_up_character_lcd_set_cursor_pos(de2_lcd, 0, 1);
	alt_up_character_lcd_string(de2_lcd, "ACKNOWLEDGEMENT.");
}

int push_LCD_init(alt_up_character_lcd_dev* tutorial_lcd)
{
    alt_up_character_lcd_init(tutorial_lcd);
    return 0;
}

void init_push_irq(void * function) {
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0x0);
	alt_irq_register((alt_u32)KEYS_IRQ, NULL, function);
}

int main()
{
	// when display needs to be updated, do so (see string that's sent)
	// alt_irq_register( (alt_u32)UART_0_IRQ, NULL, process_Serial );
	// setup
    alt_up_character_lcd_dev* de2_lcd = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
	alt_alarm * serial_alarm;
	serial * de22pi_rs232 = initialize_serial(RS232_0_0_BASE, RS232_0_0_NAME,
			RS232_0_0_IRQ, RS232_0_0_IRQ_INTERRUPT_CONTROLLER_ID);
	if(de22pi_rs232 == NULL && DEBUG) {
		printf("Serial initialisation for %s failed.\n", RS232_0_0_NAME);
		printf("Exiting...\n");
		return -1;
	}

	int errno;
	if((errno = alt_alarm_start(serial_alarm, PERIOD_SECONDS * TIMER_0_FREQ,
			serial_read, (void *) de22pi_rs232))== 0)
	{
		// somebody set us up the bomb #zerowing
		// the alarm is set up. properly so we
		alt_avalon_timer_sc_init(TIMER_0_BASE,
				TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,
				TIMER_0_IRQ,
				TIMER_0_FREQ);
	}
	else if (DEBUG)
	{
		printf("Alarm initialisation failed with error %d.\n", errno);
		printf("Exiting...\n");
		return errno;
	}
    init_push_irq(push_isr);

    // main section
    while (1) {
    	if(button_pushed)
    	{
    		serial_write(de22pi_rs232, PUSH_MSG);
    		button_pushed=0;
    		char_lcd_clear();
    	}
    	if(de22pi_rs232->msg_read) {
    		alt_up_character_lcd_string(de2_lcd, de22pi_rs232->read_message);
    		de22pi_rs232->msg_read = 0;
    	}
    }
    return 0;
}
