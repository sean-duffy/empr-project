#include "synth.h"

//Instrument declarations
struct Voice voice_template;
struct Voice voice_1;
struct Voice voice_2;
struct Voice voice_3;
struct Voice voice_4;
struct Voice voice_5;
struct Voice voice_6;

//Instrument initialisers
void init_voice_template();
void init_voice_1();
void init_voice_2();
void init_voice_3();
void init_voice_4();
void init_voice_5();
void init_voice_6();

//...
void set_voice_by_id(int voice_id, double wave_buf_1[], double wave_buf_2[]);

