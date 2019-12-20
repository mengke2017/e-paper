CC=arm-linux-gnueabihf-gcc
#CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
CFLAGS = -O2 -g -Wall -fmessage-length=0
LIBS = -I . -lfreetype  -lm

TARGET = e-paper

$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
