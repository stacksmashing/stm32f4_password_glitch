#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "api.h"
#include "api-asm.h"
#include <libopencm3/stm32/usart.h>
#include "sha256.h"

char *read_line(void);
char line[256];
char *read_line(void) {
	uint32_t received = 0;
	memset(line, 0, 256);
	bool n_received = false;
	while(true) {
		char c;
		usart_read_blocking(USART1, &c, 1);
		if((c == '\r') || (c == '\n')) {
			return line;
		}
		if(received >= 255) {
			return line;
		}
		line[received] = c;
		received += 1;
	}
}
char user[256];
char password[256];


char * correct_password = "\xed`\xc3\x4e\xbe\x21\x41\x92\xec\xd8\x95\xc1\xc3\x48\x14\xc9\xd1\x9d\x97\xa2\xb2\x39\x20\x90\x9b\xce\x07\x4e\xee\xce\xed\x4d";

int main(void) {
	// Uncomment to enable USART on PA9 (TX) and PA10 (RX)
	usart_setup();

	// Enable GPIOB clock
	rcc_periph_clock_enable(RCC_GPIOC);

	// Set PB13 as output
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);

	// Clear PB13
	gpio_clear(GPIOC, GPIO11);

	while(1)  {
		usart_send_string("Password: ");
		char * pass_input = read_line();
		strcpy(password, pass_input);
		usart_send_string("\r\n");
		// Hash password
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, password, strlen(password));
		char hash_out[32];
		sha256_final(&ctx, hash_out);

		if(memcmp(hash_out, correct_password, 32) == 0) {
			usart_send_string("Correct password! The flag is: HXT{GlitchedThePasswordCheck}\r\n");
		} else {
			usart_send_string("Incorrect password!\r\n");
		}
	}
}
