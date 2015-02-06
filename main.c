#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_can.h"

#include "CAN/can.h"
#include "UART/uart.h"
#include "SYNTH/oscillator.h"
#include "SYNTH/synth.h"

CAN_MSG_Type RXMsg;
struct Voice voice_1;

void CAN_IRQHandler(void)
{
    uint8_t IntStatus = CAN_IntGetStatus(LPC_CAN2);

    if((IntStatus>>0)&0x01)
    {
       CAN_ReceiveMsg(LPC_CAN2, &RXMsg);
       if(RXMsg.len == 0) { 
            //write_serial("Received text S/E\n\r", 19);
        } else if (RXMsg.len == 5) {
            uint8_t channel = RXMsg.dataA[0];
            uint8_t note = RXMsg.dataA[1];
            uint8_t volume = RXMsg.dataA[2];
            uint8_t type = RXMsg.dataA[3];
            uint8_t control = RXMsg.dataB[0];
            char toPrint[50];

            if (channel == 2) {
                if (volume == 0) {
                    rest(0);
                } else {
                    play_note(voice_1, get_freq(note), 500);
                }

                //write_serial("Received MIDI\n\r", 15);

                //int recValLength = sprintf(toPrint, "Channel: %d\r\n", channel);      
                //write_serial(toPrint, recValLength);

                //recValLength = sprintf(toPrint, "Note: %d\r\n", note);
                //write_serial(toPrint, recValLength);

                //recValLength = sprintf(toPrint, "Volume: %d\r\n", volume);
                //write_serial(toPrint, recValLength);

                //recValLength = sprintf(toPrint, "Type: %d\r\n", type);
                //write_serial(toPrint, recValLength);

                //recValLength = sprintf(toPrint, "Control: %d\r\n", control);
                //write_serial(toPrint, recValLength);

                //write_serial("\r\n", 2);
            }
        }

        else if(RXMsg.len == 8) {
            //write_serial("Received text\r\n", 15);
        } else {
            //write_serial("WTF\r\n", 5); 
        }   
    }
    
}

void CAN_InitMessage(void) {
    RXMsg.format = 0x00;
    RXMsg.id = 0x00;
    RXMsg.len = 0x00;
    RXMsg.type = 0x00;
    RXMsg.dataA[0] = RXMsg.dataA[1] = RXMsg.dataA[2] = RXMsg.dataA[3] = 0x00000000;
    RXMsg.dataB[0] = RXMsg.dataA[1] = RXMsg.dataA[2] = RXMsg.dataA[3] = 0x00000000;
}

void main()
{
    init_dac();
    int resolution = 360;
    set_resolution(resolution);
 
    double voice_sawtooth[resolution];
    generate_sawtooth(voice_sawtooth, resolution);

    voice_1.osc_1_buf = voice_sawtooth;
    voice_1.osc_1_mix = 0.5;
    voice_1.osc_2_buf = voice_sawtooth;
    voice_1.osc_2_mix = 0;
    voice_1.osc_2_detune = 0;

    set_voice(voice_1);

    //play_note(voice_1, 500, 125);
 
    CAN_InitMessage();
    LPC_GPIO0->FIODIR |= (1 << 10);
 
    LPC_GPIO0->FIOSET &= 0b111111111111111110111111111;
 
    //serial_init();
    //write_serial("booted\n\r", 8);
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 4;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 5;
    PINSEL_ConfigPin(&PinCfg);

    CAN_Init(LPC_CAN1, 250000);
    CAN_Init(LPC_CAN2, 250000);

    CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);
    CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);
    NVIC_EnableIRQ(CAN_IRQn);

    CAN_SetAFMode(LPC_CANAF, CAN_AccBP);

    //write_serial("loaded\n\r", 8);
    int i = 0, j = 0;
    char outVal[30];



    //int n;
    //double freq;

    //double Cm[] = {40, 43, 47, 52, 55, 59, 64, 67, 71};
    //double Bb[] = {38, 42, 45, 50, 54, 57, 62, 66, 69};
    //double Fm[] = {33, 36, 40, 45, 48, 52, 57, 60, 64};

    //double *arps[] = {Cm, Bb, Fm, Cm, Bb, Fm, Cm, Cm};

    //while (1) {
    //    for (n = 0; n < 8; n++) {
    //        for (i = 0; i < 9; i++) {
    //            freq = get_freq(arps[n][i]);
    //            play_note(voice_1, freq, 125);
    //        }
    //        for (i = 7; i > 0; i--) {
    //            freq = get_freq(arps[n][i]);
    //            play_note(voice_1, freq, 125);
    //        }
    //    }
    //}

}
