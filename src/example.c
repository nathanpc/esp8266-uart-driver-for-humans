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
 * Application entry point.
 */
void ICACHE_FLASH_ATTR user_init() {
	// Set the baud rate and also move all debug information (os_printf) to UART1.
	uart_initialize(true, 115200, 115200);
	os_delay_us(10000);                     // Baud change takes a while to take effect.
	os_printf("\r\n");                      // Separate from the boot log.

//uart_rx_intr_enable(uint8 uart_no)

#ifdef DEBUG
	os_printf("\r\n");
	print_system_info();

	os_printf("\r\n");
	struct rst_info *resinfo = system_get_rst_info();
	print_reset_info(resinfo);
#endif

	os_printf("\r\n===== Welcome to the UART driver for humans example! =====\r\n");
}

/**
 * Prints some of the system information. Usually used on boot.
 */
void ICACHE_FLASH_ATTR print_system_info() {
	os_printf("SDK version: %s\r\n", system_get_sdk_version());
	os_printf("Chip ID: %lu\r\n", system_get_chip_id());
	os_printf("Free RAM: %d\r\n", system_get_free_heap_size());
	os_printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
}

/**
 * Prints some reset debug information that is very important if you want to
 * know if the watchdog was the cause.
 *
 * @param resinfo Reset information struct returned by system_get_rst_info.
 */
void ICACHE_FLASH_ATTR print_reset_info(struct rst_info *resinfo) {
	os_printf("Reset Reason: %d", resinfo->reason);

	switch (resinfo->reason) {
		case REASON_DEFAULT_RST:
			os_printf(" -> Normal startup by power\r\n");
			break;
		case REASON_WDT_RST:
			os_printf(" -> Hardware WDT reset\r\n");
			break;
		case REASON_EXCEPTION_RST:
			os_printf(" -> Exception reset\r\n");
			break;
		case REASON_SOFT_WDT_RST:
			os_printf(" -> Software WDT reset\r\n");
			break;
		case REASON_SOFT_RESTART:
			os_printf(" -> Software call reset\r\n");
			break;
		case REASON_DEEP_SLEEP_AWAKE:
			os_printf(" -> Deep sleep wake-up\r\n");
			break;
		case REASON_EXT_SYS_RST:
			os_printf(" -> External system reset\r\n");
			break;
		default:
			os_printf(" -> UNKNOWN CAUSE\r\n");
	}
}

