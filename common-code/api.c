#include "api.h"

#include <libopencm3/stm32/usart.h>

/* 

UART Notes

- The USART we are using is USART
- PA9 (pin 42) is set to TX
- PA10 (pin 43) is set to RX

*/
void usart_setup(void) {
	// Enable required clocks
	rcc_periph_clock_enable(CLOCK_USART_GPIO);
    rcc_periph_clock_enable(CLOCK_USART);

    // Set USART parameters
    usart_set_baudrate(USART, 115200);
    usart_set_databits(USART, 8);
    usart_set_stopbits(USART, USART_STOPBITS_1);
    usart_set_mode(USART, USART_MODE_TX_RX);
    usart_set_parity(USART, USART_PARITY_NONE);
    usart_set_flow_control(USART, USART_FLOWCONTROL_NONE);

	// Setup IO
	gpio_mode_setup(PORT_USART, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_TX | PIN_RX);

	// AF stands for alternate function. AF7 is USART for these pins
	// according to the datasheet.
	gpio_set_af(PORT_USART, GPIO_AF7, PIN_TX | PIN_RX);

    // Enable USART
    usart_enable(USART);
}


void usart_send_string(const char *str) {
    while (*str) {
        usart_send_blocking(USART, *str);
		usart_wait_send_ready (USART);
		while(!usart_get_flag(USART, USART_SR_TC));
        str++;
    }
}

void usart_read_blocking(uint32_t usart, char *buffer, size_t buffer_len) {
	for(size_t i = 0; i < buffer_len; i++) {
		buffer[i] = usart_recv_blocking(usart);
	}
}
