# Makefile for the libc stuff for our MBED system

# Global Settings
# Where is the GCC compiler for this arcitecture
# SOURCERY 	= /usr/pcc/pc007/
GNU_VERSION 	= 4.5.0
TRGT	= $(LPATH)arm-none-eabi-
CC	= $(TRGT)gcc
LD	= $(TRGT)gcc
AR	= $(TRGT)ar
RANLIB	= $(TRGT)ranlib
CP	= $(TRGT)objcopy 
AS	= $(TRGT)gcc -c  -x assembler-with-cpp
BIN	= $(CP) -O ihex
OBJCOPY	= $(TRGT)objcopy
OBJDUMP	= $(TRGT)objdump
SIZE	= $(TRGT)size
CMSIS  	= ../../../LPC1700CMSIS/
# CFLAG settings
CFLAGS   = -mcpu=cortex-m3  -mthumb  -Wall  -O0  -mapcs-frame  -D__thumb2__=1
CFLAGS  += -msoft-float  -gdwarf-2  -mno-sched-prolog  -fno-hosted  
CFLAGS  += -mtune=cortex-m3  -march=armv7-m  -mfix-cortex-m3-ldrd
CFLAGS  += -I$(CMSIS)Drivers/include -I$(CMSIS)Core/CM3/CoreSupport
CFLAGS  += -I$(CMSIS)Core/CM3/DeviceSupport/NXP/LPC17xx
CFLAGS  += -I../../include -I../../libc -I ../../syscalls -I../../nsh 
CFLAGS  += -Ifatfs -I. -I.. 

# Local Settings for this Makefile

LIBRARY	= libsdcarddrvlib.a
# removed sdcard_functions.o
# Files Needed 
OBJ	 =  fatfs/ff.o  fatfs/mmc.o fatfs/ccsbcs.o spi.o 
OBJ	+=  rootfs.o sdcardfs.o eepromfs.o

# Build the library file
all:		$(OBJ) 
		$(AR) crv $(LIBRARY) $(OBJ)
		$(RANLIB) $(LIBRARY)
		mv $(LIBRARY) ..

	
# edit the program source
edit:
	gedit --new-window Makefile *.[ch]&
	
#clean out the source tree ready to re-build
clean:
	rm -f `find . | grep \~`
	rm -f *.swp *.o */*.o  *.log
	rm -f *.d */*.d *.srec
	rm -f *.elf *.wrn *.bin
	
# what we have to do to the header files
headers: 
	$(HEADERS) $(HSRC)
