#ifndef MIDI_H
#define MIDI_H

struct midi_message
{
    uint8_t status_byte;
    uint8_t data_byte_1;
    uint8_t data_byte_2;
}

struct text_message
{
    
}

void interpret_message(CAN_MSG_Type* received_message, uint8_t debug)


#endif
