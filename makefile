OUTFILE = light_sampler
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow

app:
	$(CC_C) $(CFLAGS) light_sampler.c modules/A2D/A2D.c modules/SegDisplay/SegDisplay.c modules/tools/tools.c -o $(OUTDIR)/$(OUTFILE)

all:
	$(CC_C) $(CFLAGS) light_sampler.c -o $(OUTDIR)/$(OUTFILE)

clean:
	rm $(OUTDIR)/$(OUTFILE)