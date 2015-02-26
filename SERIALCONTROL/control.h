#ifndef CONTROL_H
#define CONTROL_H

 /* buffer size definition */
 #define UART_RING_BUFSIZE 256
 
 /* Buf mask */
 #define __BUF_MASK (UART_RING_BUFSIZE-1)
 /* Check buf is full or not */
 #define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
 /* Check buf will be full in next receiving or not */
 #define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
 /* Check buf is empty */
 #define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
 /* Reset buf */
 #define __BUF_RESET(bufidx)     (bufidx=0)
 #define __BUF_INCR(bufidx)      (bufidx=(bufidx+1)&__BUF_MASK)  

typedef struct
 {
     __IO uint32_t tx_head;                
     __IO uint32_t tx_tail;                
     __IO uint32_t rx_head;                
     __IO uint32_t rx_tail;                
     __IO uint8_t  tx[UART_RING_BUFSIZE];  
     __IO uint8_t  rx[UART_RING_BUFSIZE];  
 } UART_RING_BUFFER_T;

 // UART Ring buffer
 UART_RING_BUFFER_T rb;
 
 // Current Tx Interrupt enable state
 __IO FlagStatus TxIntStat;

void UART_IntReceive();
void UART_IntTransmit();
void UART_IntErr(uint8_t bLSErrType);


#endif
