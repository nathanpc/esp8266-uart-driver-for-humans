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

// Setup the UART receive task parameters and variables.
#define RX_TASK_PRIORITY  0
#define RX_TASK_QUEUE_LEN 10
os_event_t rx_task_queue[RX_TASK_QUEUE_LEN];
void (*_rx_char_interrupt)(char) = NULL;

/**
 * System task that deals with received characters.
 *
 * @param evt System event structure.
 */
static void ICACHE_FLASH_ATTR rx_task(os_event_t *evt) {
	if (evt->sig == 0) {
#if  UART_BUFF_EN  
		Uart_rx_buff_enq();
#else
		uint8 i = 0;
		uint8 fifo_len = (READ_PERI_REG(UART_STATUS(UART0)) >> UART_RXFIFO_CNT_S)
			& UART_RXFIFO_CNT;
		
		for (i = 0; i < fifo_len; i++) {
			char c = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
			if (_rx_char_interrupt != NULL) {
				_rx_char_interrupt(c);
			}
		}

		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR |
				UART_RXFIFO_TOUT_INT_CLR);
		uart_rx_intr_enable(UART0);
#endif
	}
}


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

	// Setup a system task to deal with the received data.
	system_os_task(rx_task, RX_TASK_PRIORITY, rx_task_queue, RX_TASK_QUEUE_LEN);

	// Print debug via UART1 if the user prefers.
	if (debug_on_uart1) {
		UART_SetPrintPort(UART1);
	}
}

/**
 * Sets the received character interrupt function.
 *
 * @param int_func Interrupt function pointer.
 */
void ICACHE_FLASH_ATTR uart_set_rx_char_interrupt(void (*int_func)(char)) {
	_rx_char_interrupt = int_func;
}

/**
 * Transmits a single character via UART0.
 *
 * @param c Character to be sent.
 */
void uart_putc(char c) {
	uart_tx_one_char(UART0, c);
}

/**
 * Transmits a full string via UART0.
 *
 * @param str String to be sent.
 */
void uart_print(char *str) {
	while (*str) {
		uart_tx_one_char(UART0, *str++);
	}
}

/**
 * Sets which port will output the debug messages from os_printf.
 *
 * @param port Which port to send debug messages to.
 */
void ICACHE_FLASH_ATTR uart_set_debug_port(uint8 port) {
	UART_SetPrintPort(port);
}

