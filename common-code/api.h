#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define USART USART1
#define PORT_USART GPIOA
#define CLOCK_USART_GPIO RCC_GPIOA
#define CLOCK_USART RCC_USART1
#define PIN_TX GPIO9
#define PIN_RX GPIO10

void usart_setup(void);
void usart_send_string(const char *str);
