#ifndef CAN_H
#define CAN_H

void init_can(LPC_CAN_TypeDef* can_interface, uint8_t id, uint32_t baud_rate);
void set_receive_filter(LPC_CAN_TypeDef* can_interface, unsigned int filter);
void enable_interrupt(LPC_CAN_TypeDef* can_interface);
void disable_interrupt(LPC_CAN_TypeDef* can_interface);

#endif
