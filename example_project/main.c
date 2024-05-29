#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "api.h"
#include "api-asm.h"
#include <libopencm3/stm32/usart.h>

void usart_setup(void);
void usart_send_string(const char *str);


void glitch_loop(void);





#pragma GCC push_options
#pragma GCC optimize("O0")
#define OUTER_LOOP_CNT 300
#define INNER_LOOP_CNT 300
void glitch_loop() {
	register volatile uint32_t i;
	i = 0;

    uint32_t f = 0x12345678;
    uint32_t * src = &f;
  
    uint32_t dest[8] = {0};
	// Trigger signal
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	gpio_set(GPIOC, GPIO11);
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	gpio_clear(GPIOC, GPIO11);
__asm volatile(
        "mov r0, #0\n"
        // "mov r1, #0\n"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
        "ldm %1, {r0}\n"
        "nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"
		"nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;"

        "stm %0, {r0-r7}\n"
        :                            // No outputs
        : "r" (dest), "r" (&f)      // Input operands
        :  "r0", "r1",  "memory"             // Clobber list, r1 is modified, memory is affected
    );
	gpio_set(GPIOC, GPIO11);
	
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	__asm__("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
	// gpio_clear(GPIOD, GPIO2);
	gpio_clear(GPIOC, GPIO11);

    if((dest[1] == 0x12345678) || (dest[2] == 0x12345678) || (dest[3] == 0x12345678) || (dest[4] == 0x12345678) || (dest[5] == 0x12345678)) {
      usart_send_string("!SUC");

    } else if(dest[0] != 0x12345678) {
      usart_send_string("!SKI");
    } else {
      usart_send_string("!REG");
    }


	while(1) {
		
	}
}


int main(void) {
	// Uncomment to enable USART on PA9 (TX) and PA10 (RX)
	// usart_setup();

	// Enable GPIOB clock
	rcc_periph_clock_enable(RCC_GPIOB);

	// Set PB13 as output
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);

	// Clear PB13
	gpio_clear(GPIOB, GPIO13);

	while(1) {
		// Toggle PB13
		gpio_toggle(GPIOB, GPIO13);

		// To also send a string on USART, uncomment this line:

	}
}
