/**
 * example.c
 * Sane UART library example.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#define DEBUG

#include <c_types.h>
#include <user_interface.h>
#include <osapi.h>
#include <os_type.h>

// Include the library.
#include "sane_uart.h"

// Function prototypes.
void ICACHE_FLASH_ATTR print_system_info();
void ICACHE_FLASH_ATTR print_reset_info(struct rst_info *resinfo);

/**
 * Character received interrupt.
 *
 * @param c Character received.
 */
void rx_char_interrupt(char c) {
	// Go something!
	uart_print("< ");
	uart_putc(c);
	uart_print("\r\n");
}

/**
 * Application entry point.
 */
void ICACHE_FLASH_ATTR user_init() {
	// Move all debug information (including calls to os_printf) to UART1.
	uart_set_debug_port(1);

	// Set the baud rate.
	uart_initialize(false, 115200, 115200);
	os_delay_us(10000);  // Baud change takes a while to work some times.

	// Set a interrupt function to be called when a character is received.
	uart_set_rx_char_interrupt(rx_char_interrupt);

	// Print characters with this function:
	uart_putc('\r');
	uart_putc('\n');
	
	// Print strings using this function:
	uart_print("\r\n===== Welcome to the UART driver for humans example! =====\r\n");
}

