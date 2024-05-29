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
void receive(void) {
}
void glitch_loop() {
	// Trigger signal
	TRIGGER

	volatile register uint32_t result = 0xAA;
	__asm volatile(
		"mov r1, #1\n"        // Set r1 to 1
		
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop;" // 1 less nop so timing is similar to the others
		"cmp r1, #1\n"        // Compare r1 with 1
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop;" 
		"bne not_equal\n"     // Branch if not equal
		"mov %[output], #1\n" // If equal, set result to 1 (true)
		"b done\n"
	"not_equal:\n"
		"mov %[output], #0\n" // If not equal, set result to 0 (false)
	"done:"
		: [output] "=r" (result) // Output
		: // No input
		: "r1", "cc" // Clobbered registers
	);

	TRIGGER

	if(result) {
		usart_send_string("!REG");
	} else {
		usart_send_string("!SUC");
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
