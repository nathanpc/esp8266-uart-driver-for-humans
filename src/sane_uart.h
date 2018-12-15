/**
 * sane_uart.h
 * Sane UART driver for ESP8266.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef SANE_UART_H
#define SANE_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <os_type.h>

// Interrupts
void ICACHE_FLASH_ATTR uart_set_rx_char_interrupt(void (*int_func)(char));

// Configuration
void ICACHE_FLASH_ATTR uart_set_debug_port(uint8 port);

// Initialization
void ICACHE_FLASH_ATTR uart_initialize(bool debug_on_uart1, uint32 baud_uart0,
		uint32 baud_uart1);

// Printing
void uart_putc(char c);
void uart_print(char *str);

#ifdef __cplusplus
}
#endif 

#endif  // SANE_UART_H

