OUTFILE = light_sampler
DBUG_OUTFILE = noworky
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
OBJECTS = main.c modules/A2D/A2D.c modules/SegDisplay/SegDisplay.c modules/tools/tools.c modules/CircularBuffer/CircularBuffer.c modules/periodTimer/periodTimer.c modules/UDP/UDP.c
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow
DBUG_FLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

app:
	$(CC_C) $(CFLAGS) $(OBJECTS) -o $(OUTDIR)/$(OUTFILE) -pthread

noworky:
	$(CC_C) $(DBUG_FLAGS) noworky.c -o $(OUTDIR)/$(DBUG_OUTFILE)

all: app noworky

clean:
	rm -f $(OUTDIR)/$(OUTFILE) $(OUTDIR)/$(DBUG_OUTFILE)