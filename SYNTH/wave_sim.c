#include "wave_sim.h"

void init_print(){
	FILE *t = (fopen("temp.txt", "w"));
	f = t;
	if (f == NULL){
    printf("Error opening file!\n");
    exit(1);
	}
	fprintf(f, "[");
}

void close_print(){
	fprintf(f, "1");
	fprintf(f, "]");
	fclose(f);
}

int plot_print(double val){
	fprintf(f, "%f,", val);
}
