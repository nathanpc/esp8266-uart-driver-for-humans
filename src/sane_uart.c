/**
 * sane_uart.c
 * Sane UART driver for ESP8266.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "sane_uart.h"

#include <c_types.h>
#include <os_type.h>
#include <osapi.h>

#include "driver/uart.h"

/**
 * Initializes the UART peripheral and configures the baud rate for both ports
 * and also if debug information (os_printf) should be sent to UART1.
 *
 * @param debug_on_uart1 TRUE if you want debug information to be output via
 *                       UART1.
 * @param baud_uart0     Baud rate for UART0.
 * @param baud_uart1     Baud rate for UART1.
 */
void ICACHE_FLASH_ATTR uart_initialize(bool debug_on_uart1, uint32 baud_uart0,
		uint32 baud_uart1) {
	// Initialize the UART peripherals and set the baud rates.
	uart_init(baud_uart0, baud_uart1);

	// Print debug via UART1 if the user prefers.
	if (debug_on_uart1) {
		UART_SetPrintPort(UART1);
	}
}

