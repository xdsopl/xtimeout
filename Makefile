CFLAGS = -std=c99 -W -Wall -O3 -D_POSIX_C_SOURCE
LDFLAGS = -lxcb -lxcb-screensaver

xtimeout: xtimeout.c

test: xtimeout
	./xtimeout 3 xclock

clean:
	rm -f xtimeout

