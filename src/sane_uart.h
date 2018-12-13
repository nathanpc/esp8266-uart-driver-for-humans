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

// Public functions.
void ICACHE_FLASH_ATTR uart_initialize(bool debug_on_uart1, uint32 baud_uart0,
		uint32 baud_uart1);

#ifdef __cplusplus
}
#endif 

#endif  // SANE_UART_H

