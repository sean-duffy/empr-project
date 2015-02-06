#ifndef UART_H
#define UART_H

int read_serial(char *buf,int length); //non blocking
int write_serial(char *buf,int length); //blocking
void serial_init(void);



#endif
