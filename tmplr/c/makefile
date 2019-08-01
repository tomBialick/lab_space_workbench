# Makefile for tmplr
# Written by Thomas Bialick
# Started on 8/1/2019
#
#
# Run by executing in this directory:
# $ make
# In case of failure, verify gcc is installed

# No idea what this is for, so commented out
#SHELL=/bin/bash

# Set compilation options:
#
# -O0 no optimizations; remove after debugging
# -std=c99 use C99 Standard features
# -Wall show "all" warnings
# -W show even more warnings (annoying)
# -ggdb3 add extra debug info; remove after debugging
#
CC=gcc
CFLAGS=-O0 -std=c99 -Wall -W -g -ggdb3
OBJECTS= tmplr_main.o tmplr_parser.o

tmplr: $(OBJECTS)
	$(CC) $(CFLAGS) -o tmplr $(OBJECTS)

tmplr_main.o: tmplr_main.c tmplr_parser.o
	$(CC) $(CFLAGS) -c tmplr_main.c

tmplr_parser.o: tmplr_parser.c tmplr_parser.h
	$(CC) $(CFLAGS) -c tmplr_parser.c

#Clean it up
clean: rm -f *.o *.stackdump
