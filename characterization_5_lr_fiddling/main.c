#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "api.h"
#include "api-asm.h"
#include <libopencm3/stm32/usart.h>

#define TRIGGER gpio_set(GPIOC, GPIO11);\
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;"\
	"NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");\
	gpio_clear(GPIOC, GPIO11);


void glitch_loop(void);
void success_function(void);
void glitchable(void);

#pragma GCC push_options
#pragma GCC optimize("O0")

void success_function(void) {
	usart_send_string("!SUC");	
}

void glitchable(void) {
	uint32_t ptr  = &success_function;
}

void glitch_loop() {
    uint32_t check_constant = 0x12345678;
  
    uint32_t register_output[6] = {0};
	// Trigger signal
	TRIGGER
	__asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
	);
	glitchable();
	__asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
	);
	TRIGGER
	while(1) {
		
	}
}

#pragma GCC pop_options

int main(void) {
	// Uncomment to enable USART on PA9 (TX) and PA10 (RX)
	usart_setup();

	// Enable GPIOC clock
	rcc_periph_clock_enable(RCC_GPIOC);

	// Set PC11 as output
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);

	// Clear PC11
	gpio_clear(GPIOC, GPIO11);

	usart_send_string("!RST");

	while(1) {
		glitch_loop();
	}
}
