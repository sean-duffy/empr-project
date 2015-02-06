#ifndef MIDI_H
#define MIDI_H

/*
struct CAN_return_data
{
    struct midi_message midi_data;
    struct instruments channel_data;
    uint8_t is_midi;
};

struct midi_message
{
    uint8_t channel;
    uint8_t note;
    uint8_t volume;
    uint8_t type;
    uint8_t control;
};

struct instruments
{
    char ch0[30];
    char ch1[30];
    char ch2[30];
    char ch3[30];
    char ch4[30];
    char ch5[30];
    char ch6[30];
    char ch7[30];
    char ch8[30];
    char ch9[30];
    char ch10[30];
    char ch11[30];
    char ch12[30];
    char ch13[30];
    char ch14[30];
    char ch15[30];
};
*/
void interpret_message(CAN_MSG_Type* received_message, uint8_t debug);


#endif
