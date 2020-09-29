#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile int FLAG_A = 0;
volatile int FLAG_B = 0;
volatile int FLAG_C = 0;

__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

__attribute__((__interrupt__)) static void interrupt_J3(void){ 
	
	if (gpio_get_pin_interrupt_flag(TEST_A)){
		gpio_clear_pin_interrupt_flag(TEST_A);
		FLAG_A = 1;
	
	} else if (gpio_get_pin_interrupt_flag(TEST_B)){
		gpio_clear_pin_interrupt_flag(TEST_B);
		FLAG_B = 1;
	
	} else if (gpio_get_pin_interrupt_flag(TEST_C)){
		gpio_clear_pin_interrupt_flag(TEST_C);
		FLAG_C = 1;
	
	}
	return;
	
}





int main (void){
    init();
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	
	cpu_irq_disable();
	gpio_enable_pin_interrupt(TEST_A, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_enable_pin_interrupt(TEST_B, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	gpio_enable_pin_interrupt(TEST_C, GPIO_DIR_INPUT | GPIO_INIT_HIGH);
	cpu_irq_enable();
	
	
    
    while(1){
			
		
		
		if (FLAG_A){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_A);
			FLAG_A = 0;
		}
		if (FLAG_B){
			busy_delay_us(100);
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_B);
			FLAG_B = 0;
		}
		if (FLAG_C){
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_pin_high(RESPONSE_C);
			FLAG_C = 0;
		}
        //gpio_toggle_pin(LED0_GPIO);
        //printf("TEST_ A: %d\n RESPONSE_A: %d\nTEST_ C: %d\n RESPONSE_C: %d\n",TEST_A, RESPONSE_A, TEST_C, RESPONSE_C);
		//busy_delay_ms(500);
 
	}
	return 0;
}

/* TASK B
if (!gpio_get_pin_value(TEST_A)){
gpio_set_pin_low(RESPONSE_A);
busy_delay_us(5);
gpio_set_pin_high(RESPONSE_A);
}
if (!gpio_get_pin_value(TEST_B)){
gpio_set_pin_low(RESPONSE_B);
busy_delay_us(5);
gpio_set_pin_high(RESPONSE_B);
}
if (!gpio_get_pin_value(TEST_C)){
gpio_set_pin_low(RESPONSE_C);
busy_delay_us(5);
gpio_set_pin_high(RESPONSE_C);
}*/