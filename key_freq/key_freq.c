#include <stdio.h>
#include <math.h>

int getFreq(int key_n){
    // Converts piano key to freq
    float f = pow(2,(key_n - 49)/(float) 12) * 440;
    return (int) f;
}
int main(){
    printf("Hello key 10: %d\n", getFreq(10));
    return(0);
}
