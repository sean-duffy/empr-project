gcc -c instruments.c -o object/instruments.o
gcc -c oscillator.c -o object/oscillator.o
gcc -c synth.c -o object/synth.o
gcc -c wave_sim.c -o object/wave_sim.o
gcc synth.c -o bin/synth object/instruments.o object/oscillator.o object/wave_sim.o
pause
cd ./bin/
synth.exe
python graph.py