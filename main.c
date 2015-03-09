#include <string.h>
#include <stdio.h>

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_gpio.h"

#include "CAN/can.h"
#include "UART/uart.h"
#include "SYNTH/oscillator.h"
#include "SYNTH/synth.h"
#include "SYNTH/instruments.h"
#include "MIDI/midi.h"

#include "I2C/i2c.h"
#include "LCD/lcd.h"
#include "keypad/keypad.h"
#include "LED/led.h"
#include "RIT/rit.h"


int ledValue = 0;
int debug = 1;
CAN_MSG_Type RXMsg;
struct CAN_return_data message;

#define debug_print(n, x) if(debug) { write_serial(n, x); write_serial("\n\r", 2); }
#define debug_print_nnl(n, x) if(debug) { write_serial(n, x); }
uint8_t channel_playing = 1;
int voice_playing = 5;
char status_string[16];
char status_string_formatter[] = "Ch:%2d  In: %d  #%f";
char space_string[] = "                ";
char *first_line;

double wave_buf_1[RESOLUTION];
double wave_buf_2[RESOLUTION];

/*personal project starts here*/

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

#define SERIALSIZE 100

#define NAMEREQ 6
#define VOLREQ 7
#define CHANREQ 8
#define INSTREQ 9

#define VOLDATA  0 
#define CHANDATA  1 
#define INSTDATA  2 
#define NAMEDATA  3


uint8_t serialArray[SERIALSIZE];
uint8_t serialCount = 0;

uint8_t recCommand, recDataLen;
char recPayload[100];


void sendSongTitle()
{
    if(first_line == NULL)
    {
        char toSend[] = {0xFF, 19, NAMEDATA, '(', 'W', 'a', 'i', 't', 'i', 'n', 'g', ' ', 'f', 'o', 'r', ' ', 'n', 'a', 'm', 'e', ')', '\0', 0xEE};
        write_serial(toSend, 23);
    }
    else
    {
        char toSend1[] = {0xFF, strlen(first_line), NAMEDATA};
        char toSend2[] = {0xEE};

        write_serial(toSend1, 3);
        write_serial(first_line, strlen(first_line));
        write_serial(toSend2, 1);
    }

}

void sendVol()
{
    char toSend[] = {0xFF, 1, VOLDATA, (uint8_t)('0'+((int)(output_volume * 10.0))), 0xEE};
    write_serial(toSend, 5);
}

void sendChan()
{
    char toSend[] = {0xFF, 1, CHANDATA, (uint8_t)('0'+(int)channel_playing), 0xEE};
    write_serial(toSend, 5);
}

void sendInst()
{
    char toSend[] = {0xFF, 1, INSTDATA, (uint8_t)('0'+(int)voice_playing), 0xEE};
    write_serial(toSend, 5);
}

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
                                 serialArray[serialCount++] = tmpc;
                                 __BUF_INCR(rb.rx_head);
                                
                         }
                 }
                 // no more data
                 else {
                        if(serialArray[0] == 0xFF && serialArray[serialCount-1] == 0xEE)
                        {
                            uint8_t dispChange = 0;
                            recDataLen = serialArray[1];
                            recCommand = serialArray[2];
                            if(recCommand == NAMEREQ)
                            {
                                sendSongTitle();
                            }

                            else if(recCommand == VOLREQ)
                            {
                                sendVol();
                            }

                            else if(recCommand == CHANREQ)
                            {
                                sendChan();
                            }

                            else if(recCommand == INSTREQ)
                            {
                                sendInst();
                            }

                            else if(recCommand == VOLDATA)
                            {
                                if (serialArray[3] <= 9 && serialArray[3] > 0) 
                                {
                                    output_volume = (float)serialArray[3] / 10.0;
                                    dispChange = 1;
                                }
                            }

                            else if(recCommand == CHANDATA)
                            {
                                if (serialArray[3] <= 15 && serialArray[3] > 0) 
                                {
                                    channel_playing = serialArray[3];
                                    dispChange = 1;
                                }
                            }

                            else if(recCommand == INSTDATA)
                            {
                                if (serialArray[3] <= 6 && serialArray[3] > 0) 
                                {
                                    voice_playing = serialArray[3];
                                    set_voice_by_id(voice_playing, wave_buf_1, wave_buf_2);
                                    dispChange = 1;
                                }
                            }

                            if(dispChange)
                            {
                                sprintf(status_string, status_string_formatter, channel_playing, voice_playing, output_volume * 10.0);
                                write_second_line(&I2CConfigStruct, status_string, strlen(status_string));
                            }
                            serialCount = 0;
                        }

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
         set_binary_number(1);
         while (1){
                 // For testing purpose
                 test = bLSErrType;
         }
 }

void UART0_IRQHandler(void)
 {
         uint32_t intsrc, tmp, tmp1;
 
         /* Determine the interrupt source */
         intsrc = UART_GetIntId(LPC_UART0);
         tmp = intsrc & UART_IIR_INTID_MASK;
 
         // Receive Line Status
         if (tmp == UART_IIR_INTID_RLS){
                 // Check line status
                 tmp1 = UART_GetLineStatus(LPC_UART0);
                 // Mask out the Receive Ready and Transmit Holding empty status
                 tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);

                 if (tmp1) {
                                 UART_IntErr(tmp1);
                 }
         }
 
         // Receive Data Available or Character time-out
         if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
                         UART_IntReceive();
         }
 
         // Transmit Holding Empty
         if (tmp == UART_IIR_INTID_THRE){
                         UART_IntTransmit();
         }
 }

extern void RIT_IRQHandler()
{
    clear_interrupt_flag();
    scroll_first_line(&I2CConfigStruct, first_line, strlen(first_line));
    //scroll_first_line(&I2CConfigStruct, firstLine, strlen(firstLine));

    if(ledValue == 14)
	{
		ledValue = 0;
	}
	
	set_binary_number(ledValue++);  
	
}

/*Personal ends here*/

void CAN_IRQHandler(void) {
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01) {
        CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
        interpret_message(&RXMsg, 1, &message);
        
        if (message.done) {
            NVIC_DisableIRQ(RIT_IRQn);
            set_binary_number(16);         
            first_line = (char *) calloc(strlen(message.text_data.track) - 12 + 16 + 3 + strlen(message.text_data.bpm), sizeof(char));
            strcpy(first_line, space_string);
            strncat(first_line, message.text_data.track, strlen(message.text_data.track) - 12);
            strcat(first_line, " - ");
            strcat(first_line, message.text_data.bpm);

            message.done = 0;

            NVIC_EnableIRQ(RIT_IRQn);
            sendSongTitle();
        }

        if (message.is_midi) {
            if (message.midi_data.channel == channel_playing) {
                if (message.midi_data.volume == 0) {
                    note_off();
                } else {
                    note_on(get_freq(message.midi_data.note));
                }
            }
            message.is_midi = 0;
        }
    }
}

extern void EINT3_IRQHandler() {
    char readChar = keypadRead(LPC_I2C1, keypadAddr);
    // Clear interrupt
    uint8_t data[] = {0b00001111};
    i2cWrite(LPC_I2C1, keypadAddr, data, 1);
    GPIO_ClearInt(0, 0x00800000);

    if (readChar == '9' && output_volume < 0.9) {
        output_volume += 0.1;
        sendVol();
    } else if (readChar == '#' && output_volume > 0.1) {
        output_volume -= 0.1;
        sendVol();
    }

    if (readChar == '7' && channel_playing < 15) {
        note_off();
        channel_playing += 1;
        sendChan();
    } else if (readChar == '*' && channel_playing > 1) {
        note_off();
        channel_playing -= 1;
        sendChan();
    }

    if (readChar == '8' && voice_playing < 6) {
        voice_playing += 1;
        set_voice_by_id(voice_playing, wave_buf_1, wave_buf_2);
        sendInst();
    } else if (readChar == '0' && voice_playing > 1) {
        voice_playing -= 1;
        set_voice_by_id(voice_playing, wave_buf_1, wave_buf_2);
        sendInst();
    }

    sprintf(status_string, status_string_formatter, channel_playing, voice_playing, output_volume * 10.0);
    write_second_line(&I2CConfigStruct, status_string, strlen(status_string));
}


void main() {
    serial_init();
    set_resolution(RESOLUTION);
    leds_init();
    init_ritimer(20);
    NVIC_SetPriority(RIT_IRQn, 50);
    set_voice_by_id(voice_playing, wave_buf_1, wave_buf_2);
	i2cInit(LPC_I2C1, 100000);

    init_dac();
    init_can(250000, 0);

    SysTick_Config(2400);

    I2CConfigStruct.retransmissions_max = 3;
    I2CConfigStruct.sl_addr7bit = 59;

    uint8_t lcd_init[] = {0x00, 0x35, 0x00, 0x9F, 0x00, 0x34, 0x00, 0x0C, 0x00, 0x02};
    lcd_write_bytes(&I2CConfigStruct, lcd_init, sizeof(lcd_init));

    uint8_t clear[] = {0x00, 0x01};
    lcd_write_bytes(&I2CConfigStruct, clear, sizeof(clear));

    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);

    sprintf(status_string, status_string_formatter, channel_playing, voice_playing, output_volume * 10.0);
    write_second_line(&I2CConfigStruct, status_string, strlen(status_string));
    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);

    keypadInit(LPC_I2C1, keypadAddr);
    isBusyWait(LPC_I2C1, I2CConfigStruct.sl_addr7bit);  
    NVIC_EnableIRQ(RIT_IRQn);
    while (1);
}
