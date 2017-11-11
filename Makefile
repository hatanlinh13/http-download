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
LD     = ld
INCDIR = ./include/
BINDIR = ./bin/
INCOPT = -I./include
CFLAGS = $(INCOPT)
LFLAGS =

all: main.o \
	cmd.o httpv.o ghost.o gtarget.o \
	gobject.o sockmngt.o htmlp.o filemngt.o
	$(LD) $(LFLAGS) \
		$(BINDIR)main.o $(BINDIR)cmd.o \
		$(BINDIR)httpv.o $(BINDIR)ghost.o $(BINDIR)gtarget.o \
		$(BINDIR)gobject.o $(BINDIR)sockmngt.o \
		$(BINDIR)htmlp.o $(BINDIR)filemngt.o \
		-o 1512284_1512387_1512491

httpv.o: httpv.c $(INCDIR)httpv.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c httpv.c $(BINDIR)httpv.o

ghost.o: ghost.c $(INCDIR)ghost.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c ghost.c $(BINDIR)ghost.o

gtarget.o: gtarget.c $(INCDIR)gtarget.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c gtarget.c $(BINDIR)gtarget.o

cmd.o: cmd.c $(INCDIR)cmd.h $(INCDIR)defs.h \
	$(INCDIR)httpv.h $(INCDIR)ghost.h $(INCDIR)gtarget.h
	$(CC) $(CFLAGS) -c cmd.c -o $(BINDIR)cmd.o

htmlp.o: htmlp.c $(INCDIR)htmlp.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c htmlp.c $(BINDIR)htmlp.o

filemngt.o: filemngt.c $(INCDIR)filemngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c filemngt.c $(BINDIR)filemngt.o

gobject.o: gobject.c $(INCDIR)gobject.h \
	$(INCDIR)htmlp.h $(INCDIR)filemngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c gobject.c $(BINDIR)gobject.o

sockmngt.o: sockmngt.c $(INCDIR)sockmngt.h $(INCDIR)defs.h
	$(CC) $(CFLAGS) -c sockmngt.c $(BINDIR)sockmngt.o

main.o: main.c $(INCDIR)defs.h $(INCDIR)cmd.h $(INCDIR)gobject.h $(INCDIR)sockmngt.h
	$(CC) $(CFLAGS) -c main.c $(BINDIR)main.o

clean:
	rm ./bin/* 1512284_1512387_1512491
