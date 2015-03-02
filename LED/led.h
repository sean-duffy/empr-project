#ifndef LED_H
#define LED_H

// Led 1 is P1_18
#define LED_1 (1 << 18)
// Led 2 is P1_20
#define LED_2 (1 << 20)
// Led 3 is P1_21
#define LED_3 (1 << 21)
// Led 4 is P1_23
#define LED_4 (1 << 23)

#define LED_ALL (LED_1 | LED_2 | LED_3 | LED_4)  

void leds_init (void);
void led_on (unsigned int ledstate);
void led_off (unsigned int ledstate);
void leds_all_on (void);
void leds_all_off (void);
void set_binary_number(int number);

#endif
