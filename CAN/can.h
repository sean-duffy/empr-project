#ifndef CAN_H
#define CAN_H

void init_can(LPC_CAN_TypeDef* can_interface, uint32_t baud_rate);
void set_receive_filter(LPC_CAN_TypeDef* can_interface, unsigned int filter);
void enable_interrupt(LPC_CAN_TypeDef* can_interface);
void disable_interrupt(LPC_CAN_TypeDef* can_interface);
void set_device_id(LPC_CAN_TypeDef* can_interface, uint32_t id);

#endif
