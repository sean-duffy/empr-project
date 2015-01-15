#include "can.h"

void CAN_IRQHandler(void)
 {
         uint8_t IntStatus;
         //check FullCAN interrupt enable or not
         if(CAN_FullCANIntGetStatus(LPC_CANAF)== SET)
         {       //check is FullCAN interrupt occurs or not
                 if ((CAN_FullCANPendGetStatus(LPC_CANAF,FULLCAN_IC0))
                                 ||(CAN_FullCANPendGetStatus(LPC_CANAF,FULLCAN_IC1)))
                 {
                         //read received FullCAN Object in Object Section
                         FCAN_ReadObj(LPC_CANAF, &AFRxMsg[CANRxCount]);
                         CANRxCount++;
                 }
         }
         /* get interrupt status
          * Note that: Interrupt register CANICR will be reset after read.
          * So function "CAN_IntGetStatus" should be call only one time
          */
         IntStatus = CAN_IntGetStatus(LPC_CAN2);
         //check receive interrupt
         if((IntStatus>>0)&0x01)
         {
                 CAN_ReceiveMsg(LPC_CAN2, &AFRxMsg[CANRxCount]);
                 CANRxCount++;
         }
 }

void main()
{
	init_can(LPC_CAN1)
}
