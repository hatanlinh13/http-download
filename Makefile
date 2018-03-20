# MMT - Project 1 - Socket Programming
# HTTP Download Client
#
# 15CNTN  - Team 0xff
# Members:
# 1512284 - Ha Tan Linh
# 1512387 - Do Thanh Nhon
# 1512491 - Nguyen Thanh Tan
#
# Module: Makefile

CC     = gcc
INCDIR = ./include/
BINDIR = ./bin/
INCOPT = -I./include
CFLAGS = $(INCOPT)
LFLAGS =

.PHONY: all clean makebindir
all: makebindir main.o \
	cmd.o \
	gobject.o sockmngt.o htmlp.o filemngt.o
	$(CC) $(LFLAGS) $(BINDIR)main.o $(BINDIR)cmd.o $(BINDIR)gobject.o $(BINDIR)sockmngt.o $(BINDIR)htmlp.o $(BINDIR)filemngt.o	-o 1512284_1512387_1512491

cmd.o: cmd.c $(INCDIR)cmd.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c cmd.c -o $(BINDIR)cmd.o

htmlp.o: htmlp.c $(INCDIR)htmlp.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c htmlp.c -o $(BINDIR)htmlp.o

filemngt.o: filemngt.c $(INCDIR)filemngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c filemngt.c -o $(BINDIR)filemngt.o

gobject.o: gobject.c $(INCDIR)gobject.h $(INCDIR)sockmngt.h \
	$(INCDIR)htmlp.h $(INCDIR)filemngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c gobject.c -o $(BINDIR)gobject.o

sockmngt.o: sockmngt.c $(INCDIR)sockmngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c sockmngt.c -o $(BINDIR)sockmngt.o

main.o: main.c $(INCDIR)defs.h $(INCDIR)cmd.h $(INCDIR)gobject.h $(INCDIR)sockmngt.h
	$(CC) $(CFLAGS) -c main.c -o $(BINDIR)main.o

makebindir:
	mkdir -p bin

clean:
	rm -r ./bin 1512284_1512387_1512491
