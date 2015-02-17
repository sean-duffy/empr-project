#ifndef MIDI_H
#define MIDI_H

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
    char track[100];
    char bpm[15];
    char* channel[16];
};

struct CAN_return_data
{
    int done;
    struct midi_message midi_data;
    struct instruments text_data;
    uint8_t is_midi;
};

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug, struct CAN_return_data *ret);


#endif
