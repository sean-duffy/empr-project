#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "can.h"

#define can_controller(n) (n == LPC_CAN2 ? CAN2_CTRL : CAN1_CTRL)

void init_can(LPC_CAN_TypeDef* can_interface, uint32_t baud_rate)
{
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);	

	CAN_Init(can_interface, baud_rate);
	
	CAN_SetAFMode(can_interface, CAN_AccBP); //accept all traffic on init
}

void enable_interrupt(LPC_CAN_TypeDef* can_interface)
{
	CAN_IRQCmd(can_interface, CANINT_FCE, ENABLE);  //fullcan interrupt (29bit identifiers)
	CAN_IRQCmd(can_interface, CANINT_RIE, ENABLE);  //interrupt on receive message
	NVIC_EnableIRQ(CAN_IRQn);
}
void disable_interrupt(LPC_CAN_TypeDef* can_interface)
{
	CAN_IRQCmd(can_interface, CANINT_FCE, DISABLE);
	CAN_IRQCmd(can_interface, CANINT_RIE, DISABLE);
	NVIC_DisableIRQ(CAN_IRQn);
}

void set_device_id(LPC_CAN_TypeDef* can_interface, uint32_t id)
{
	AF_SectionDef AFTable;
	FullCAN_Entry FullCAN_Table [1];
	SFF_Entry SFF_Table [1];
	SFF_GPR_Entry SFF_GPR_Table [1];
	EFF_Entry EFF_Table [1];
	EFF_GPR_Entry EFF_GPR_Table [1];
	
	CAN_SetAFMode(can_interface, CAN_AccOff); //turn off filter for setting, accept no messages
	
	EFF_Table[0].controller = can_controller(can_interface);
	EFF_Table[0].ID_29 = id;
	
	//set all the start registers to the address of the start of their respective tables, and set how many entries each has
	AFTable.FullCAN_Sec = &FullCAN_Table[0];
	AFTable.FC_NumEntry = 0;
	AFTable.SFF_Sec = &SFF_Table[0];
	AFTable.SFF_NumEntry = 0;
	AFTable.SFF_GPR_Sec = &SFF_Table[0];//&SFF_GPR_Table[0]; //documentation says to do this when table is empty
	AFTable.SFF_GPR_NumEntry = 0;
	AFTable.EFF_Sec = &EFF_Table[0];
	AFTable.EFF_NumEntry = 1;
	AFTable.EFF_GPR_Sec = &EFF_Table[0];//&EFF_GPR_Table[0];
	AFTable.EFF_GPR_NumEntry = 0;	
	
	CAN_SetAFMode(can_interface, CAN_eFCAN);
}


