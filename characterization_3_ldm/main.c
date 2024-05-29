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
    uint32_t check_constant = 0x12345678;
  
    uint32_t register_output[6] = {0};
	// Trigger signal
	TRIGGER

	__asm volatile(
        "mov r0, #0\n"
        // "mov r1, #0\n"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        "ldm %[check_constant_address], {r0}\n"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"

        "stm %0, {r0-r5}\n"
        :                            // No outputs
        : [register_output] "r" (register_output), [check_constant_address] "r" (&check_constant)      // Input operands
        :  "r0", "r1",  "memory"             // Clobber list, r1 is modified, memory is affected
    );
	TRIGGER

    if((register_output[1] == 0x12345678) || (register_output[2] == 0x12345678) || (register_output[3] == 0x12345678) || (register_output[4] == 0x12345678) || (register_output[5] == 0x12345678)) {
      usart_send_string("!SUC");

    } else if(register_output[0] != 0x12345678) {
      usart_send_string("!SKI");
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

	while(1) {
		glitch_loop();
	}
}
