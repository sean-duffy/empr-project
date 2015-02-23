gcc -c instruments.c -o object/instruments.o
gcc -c oscillator.c -o object/oscillator.o
gcc -c synth.c -o object/synth.o
gcc -c wave_sim.c -o object/wave_sim.o
gcc synth_test.c -o bin/synth_test object/instruments.o object/oscillator.o object/synth.o object/wave_sim.o
pause
cd ./bin/
synth_test.exe
python graph.py
pause