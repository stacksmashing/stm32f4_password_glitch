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

void glitch_loop() {
	register volatile uint32_t i;
	i = 0;
	// Trigger signal
	TRIGGER

	 __asm volatile(
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        "mov %0, #12\n"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        : "=r" (i)
        :
        :
    );
	TRIGGER
	if(i == 0) {
		usart_send_string("!SKI");
	} else if(i != 12) {
		usart_send_string("!SUC");
	} else {
		usart_send_string("!REG");
	}

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

	usart_send_string("!RST");

	while(1) {
		glitch_loop();
	}
}
