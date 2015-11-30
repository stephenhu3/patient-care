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
#include <fcntl.h>

#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer.h"
#include "sys/alt_alarm.h"

#include "altera_up_avalon_character_lcd.h"
#include "PatientBoard.h"
#include "sys/alt_irq.h"
#include "system.h"

#define DEBUG 1

#define BLANK_LINE "                " // something is here don't touch!
#define BASE_DEMO_DIR 0xFFFFFFFC // most pins don't matter but last eight need to be
#define KEY_0_MASK 0x01
#define KEYS (volatile char *) KEYS_BASE
#define WAIT_ACK 0
#define WAIT_PI 1
#define WAIT_PUSH 2
#define PUSH_DONE 3

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
	// initialize the ISR
	// when display needs to be updated, do so (see string that's sent)
	// use timer to figure out how long we'll turn off the alarm
	//
	// alt_irq_register( (alt_u32)UART_0_IRQ, NULL, process_Serial );
    //alt_up_character_lcd_dev* de2_lcd = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);


    int fd = open(RS232_0_0_NAME, O_NONBLOCK | O_RDWR); //Open file for reading and writing
    init_push_irq(push_isr);
    int prompt_len = 0;
    char prompt[256] = "";
    int prompt_index = 0;
    char * later_gator = "See you later, alligator!\n";
    char * pushed = "PUSHED THE BUTTON!";
    if (fd)
    {
    	if(DEBUG)
    		printf("Opened successfully.\n");

    	while (strcmp(prompt,"Bye")!=0)
    	{ // Loop until we receive "Bye"

    		if(button_pushed)
    		{
	    		write(fd,pushed,strlen(pushed));
	    		button_pushed=0;
	    	}

    		prompt_index = 0;
    		prompt[0] = '\0';
    		prompt_len = read(fd, prompt, 128);
    		if(DEBUG && prompt_len > 0)
    		{
    			printf("No. chars received:%d\n",prompt_len);
    			printf("Message:%s\n", prompt);
    		}
//    		while(serial_char != '\0')
//    		{
//    			if(button_pushed)
//    			{
//    				fprintf(fp,"%s","PUSHED THE BUTTON!");
//    				button_pushed=0;
//    			}
//    		// while((serial_char = getc(fp))!='\n') {
//    			//strcat(prompt,&serial_char);
//    			prompt[prompt_index++] = serial_char;
//    			fprintf(fp,"%c",serial_char);
//    			printf("%c", serial_char);
//    			fread(&serial_char,1,1,fp);
//    		}
//    		prompt[prompt_index++] = serial_char;
//    		printf(prompt);
    		write(fd, prompt, strlen(prompt));
    	}
    	write(fd, later_gator, strlen(later_gator));
    	if(DEBUG)
    	{
    		printf(later_gator);
    		printf("Closing file.\n");
    	}
    	close(fd);
    }
    else if(DEBUG)
    	printf("Can't open.\n");
    return 0;
}
