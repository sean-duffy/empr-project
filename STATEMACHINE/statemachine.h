#ifndef STATEMACHINE_H
#define STATEMACHINE_H

static struct Voice voice_1;
static int volumeMultiplier = 5;
static int currentChannel = 1;
static int currentState = 0;
static unsigned int stateTimeout = 0;
static int stateTimeoutActive = 0;
static int currentVoice;

static int newChannelFirstDigit;
static int newChannelSecondDigit;
//state 0 = startup/initialise/default
//   displays scrolling track name on top line and channel and volume statically on bottom

//state 1 = volume change state
//  entered when from state 0 # or * is pressed on the keypad
//  while in the state (a timeout is set) volume variable is changed between 

//state 2 = channel change state
//  entered when from state 0 C is pressed on the keypad
//  (timeout is set, in which case no change is made), otherwise user needs to enter 0-1
//  then 0-6 to set channel

//state 3 = voice change state

void initialise_system();

int getVolume();
void setVolume(int vol);
int getChannel();
void setChannel(int chan);
int getCurrentState();
void setCurrentState(int state);
int getCurrentVoice();
void setCurrentVoice(int v);

void resetStateTimeout();
void clearStateTimeout();
void incrementStateTimeout();

int getFirstNewChannelDigit();
void setFirstNewChannelDigit(int i);

int getSecondNewChannelDigit();
void setSecondNewChannelDigit(int i);

void writeNewChannel();

#endif

