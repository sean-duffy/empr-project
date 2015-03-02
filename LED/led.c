#include "LPC17xx.h"
#include "led.h"

void leds_init (void)
{
	//LPC_CONF -> PINSEL3 
	LPC_GPIO1->FIODIR |= LED_ALL;
}

// Function to turn all leds on
void leds_all_on (void)
{
	LPC_GPIO1->FIOSET = LED_ALL;
}

// Function to turn all leds off
void leds_all_off (void)
{
	LPC_GPIO1->FIOCLR = LED_ALL;
}

// Function to turn on chosen led(s)
void led_on (unsigned int ledstate)
{
	// Turn on requested LEDs
	// (ANDing to ensure we only affect the LED outputs)
	LPC_GPIO1->FIOSET = ledstate & LED_ALL;	
}

// Function to turn off chosen led(s)
void led_off (unsigned int ledstate)
{
	// Turn off requested LEDs
	// (ANDing to ensure we only affect the LED outputs)
	LPC_GPIO1->FIOCLR = ledstate & LED_ALL;
}

void set_binary_number(int number)
{
	leds_all_off();
	switch(number)
	{
		case 1:
			led_on(LED_4);
			break;
		
		case 2:
			led_on(LED_3);
			break;
		
		case 3:
			led_on(LED_4);
			led_on(LED_3);
			break;
			
		case 4:
			led_on(LED_2);
			break;
		
		case 5:
			led_on(LED_2);
			led_on(LED_4);
			break;
			
		case 6:
			led_on(LED_2);
			led_on(LED_3);
			break;
			
		case 7:
			led_on(LED_2);
			led_on(LED_3);
			led_on(LED_4);
			break;
		
		case 8:
			led_on(LED_1);
			break;
			
		case 9:
			led_on(LED_4);
			led_on(LED_1);
			break;
			
		case 10:
			led_on(LED_3);
			led_on(LED_1);
			break;
			
		case 11:
			led_on(LED_3);
			led_on(LED_1);
			led_on(LED_4);
			break;
			
		case 12: 
			led_on(LED_1);
			led_on(LED_2);
			break;
			
		case 13:
			led_on(LED_1);
			led_on(LED_2);
			led_on(LED_4);
			break;
			
		case 14:
			led_on(LED_1);
			led_on(LED_2);
			led_on(LED_3);
			break;
			
		default:
			leds_all_on();
			break;
	}
}
