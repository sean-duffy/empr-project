#include "oscillator.h"
#include "synth.h"
#include "instruments.h"
#include "wave_sim.h"

int main(void) {
    //init_dac();
    //init_timer();
	init_print();
	init_synth();
	
    double wave_buf_1[RESOLUTION];
    double wave_buf_2[RESOLUTION];
	
	printf("Running Wave Test");
	
	set_voice_by_id(1, wave_buf_1, wave_buf_2);
	
	int i;
	for(i=0; i<1000; i++){
		SysTick_Handler();
	}

	plot_print(1);
	note_on(get_freq(60));
	
	close_print();
    return 0;
}
