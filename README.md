# ESP8266 UART Driver for Humans

If you've ever tried to use the UART driver bundled with the ESP8266 Non-OS SDK you know how painful it can be to use that pile of turd. This project tries to bring sanity and simplicity to all that crap.

## Usage

To use this driver correctly simply add it to your project like you normally would with any C source and header file that is already in your project, then adjust your Makefile to compile it with your project, and you should be good to go.

```c
#include <c_types.h>
#include <user_interface.h>
#include <osapi.h>
#include <os_type.h>

// Include the library.
#include "sane_uart.h"

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
```

