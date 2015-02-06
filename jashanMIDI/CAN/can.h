#ifndef CAN_H
#define CAN_H

void init_can(uint32_t baud_rate, short unsigned int debug_flag);
void set_receive_filter(unsigned int filter);
void enable_interrupt();
void disable_interrupt();
void set_device_id(uint32_t id);

#endif
