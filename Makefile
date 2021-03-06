#
#
#  Red Pitaya Api example Makefile
#
#
# Versioning system
VERSION ?= 0.00-0000

REVISION ?= devbuild

CROSS_COMPILE = arm-linux-gnueabi-

#Define header includes
RP_PATH_INCLUDE = -I include/

#Define library includes
RP_LIB_INCLUDE = -L include/ -lm -lpthread -lrp

#Cross compiler definition
CC = $(CROSS_COMPILE)gcc
#Flags
CFLAGS = -g -std=gnu99 -Wall -Werror
#Objects
OBJECTS = server.o connection_handler.o acquire_to_socket.o str_to_enum.o generate_arbitrary_waveform.o
#Target file
TARGET = server
#Dependancies
DEPS = server.h connection_handler.h acquire_to_socket.h str_to_enum.h generate_arbitrary_waveform.h

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(RP_LIB_INCLUDE)

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $(RP_PATH_INCLUDE) $< -o $@

#Build the executable
all: $(TARGET)

clean:
	$(RM) $(TARGET) *.o ~* 
