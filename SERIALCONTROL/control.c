#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_gpio.h"
#include "UART/uart.h"
#include <string.h>
#include <stdio.h>
#include "control.h"

void UART_IntReceive(void)
 {
         uint8_t tmpc;
         uint32_t rLen;
 
         while(1){
                 // Call UART read function in UART driver
                 rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &tmpc, 1, NONE_BLOCKING);
                 // If data received
                 if (rLen){
                        //write_serial(&tmpc, 1);
                         /* Check if buffer is more space
                          * If no more space, remaining character will be trimmed out
                          */
                         if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
                                 rb.rx[rb.rx_head] = tmpc;
                                 __BUF_INCR(rb.rx_head);
                         }
                 }
                 // no more data
                 else {
                        write_serial("hit\r\n", 5);
                        int loopCount = 0;
                        while(loopCount < UART_RING_BUFSIZE)
                        {
                            write_serial(&rb.rx[loopCount++], 2);
                        }
                        write_serial("\r\n", 2);
                        break;
                 }
         }
 }

 void UART_IntTransmit(void)
 {
     // Disable THRE interrupt
     UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
 
         /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
          * of data or break whenever ring buffers are empty */
         /* Wait until THR empty */
     while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART0) == SET);
 
         while (!__BUF_IS_EMPTY(rb.tx_head,rb.tx_tail))
     {
         /* Move a piece of data into the transmit FIFO */
         if (UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&rb.tx[rb.tx_tail], 1, NONE_BLOCKING)){
         /* Update transmit ring FIFO tail pointer */
         __BUF_INCR(rb.tx_tail);
         } else {
                 break;
         }
     }
 
     /* If there is no more data to send, disable the transmit
        interrupt - else enable it or keep it enabled */
         if (__BUF_IS_EMPTY(rb.tx_head, rb.tx_tail)) {
         UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
         // Reset Tx Interrupt state
         TxIntStat = RESET;
     }
     else{
         // Set Tx Interrupt state
                 TxIntStat = SET;
         UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, ENABLE);
     }
 }

void UART_IntErr(uint8_t bLSErrType)
 {
         uint8_t test;
         // Loop forever
         while (1){
                 // For testing purpose
                 test = bLSErrType;
         }
 }
