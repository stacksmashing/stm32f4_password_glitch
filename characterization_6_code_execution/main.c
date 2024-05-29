#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "api.h"
#include "api-asm.h"
#include <libopencm3/stm32/usart.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TRIGGER gpio_set(GPIOC, GPIO11);\
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");\
	gpio_clear(GPIOC, GPIO11);


void glitch_loop(void);
void success_function();
void usart_read_blocking(uint32_t usart, char *buffer, size_t buffer_len);


void usart_read_blocking(uint32_t usart, char *buffer, size_t buffer_len) {
	for(size_t i = 0; i < buffer_len; i++) {
		buffer[i] = usart_recv_blocking(usart);
	}
}

char local_buf[256];

void success_function() {
	usart_send_string("!SUC");	
}

void glitch_loop() {
    uint32_t check_constant = 0x12345678;
  
	__asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
	);
	
	// Number of 4-byte items to receive
	uint32_t received;
	usart_read_blocking(USART1, &received, 4);
	if((received * 4) > 256) {
		while(1) {
			
		}
	}
	char *buf = malloc(received * 4);
	usart_read_blocking(USART1, buf, received * 4);

	// Trigger signal
	TRIGGER

	__asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
	);
	
	memcpy(local_buf, buf, received * 4);
	
	__asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
	);
	TRIGGER

	while(1) {

	}
}


int main(void) {
	// Uncomment to enable USART on PA9 (TX) and PA10 (RX)
	usart_setup();

	// Enable GPIOC clock
	rcc_periph_clock_enable(RCC_GPIOC);

	// Set PC11 as output
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);

	// Clear PC11
	gpio_clear(GPIOC, GPIO11);


	// Hack to make sure success_function doesn't get removed
	volatile bool always_true = 1;
	while(always_true) {
		glitch_loop();	
	}
	// Never reached
	success_function();
}
