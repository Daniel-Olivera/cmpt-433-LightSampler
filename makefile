OUTFILE = light_sampler
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
OBJECTS = main.c modules/A2D/A2D.c modules/SegDisplay/SegDisplay.c modules/tools/tools.c modules/CircularBuffer/CircularBuffer.c modules/periodTimer/periodTimer.c
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow

app:
	$(CC_C) $(CFLAGS) $(OBJECTS) -o $(OUTDIR)/$(OUTFILE) -pthread

all:
	$(CC_C) $(CFLAGS) $(OBJECTS) -o $(OUTDIR)/$(OUTFILE) -pthread

clean:
	rm $(OUTDIR)/$(OUTFILE)