#include "synth.h"
#define resolution 360 // This needs to be overhauled (put in synth)

//Instrument declarations
struct Voice voice_template;

//Instrument initialisers
void init_voice_template();

// Array of all voice initialisers
void (*all_voices[]) (void) = { init_voice_template };

//Global voice initialiser
void init_all_voices();
