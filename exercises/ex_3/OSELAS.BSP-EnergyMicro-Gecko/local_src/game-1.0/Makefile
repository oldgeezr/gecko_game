#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
#CC = arm-cortexm3-uclinuxeabi-gcc
CFLAGS += -O3 -g -Wall -std=c99 #-D_POSIX_C_SOURCE=199309L #-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS += -lm


OBJ = display.o graphics.o


all: game

game: ${OBJ}
	${CC} ${CFLAGS} game.c ${OBJ} -o game ${LDFLAGS}

clean:
	-rm -f game *.o

install:


.PHONY: all install clean

# end of development makefile
