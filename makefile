#This is the Makefile for the airport project
CC = gcc
CFLAGS = -g -Wall
CCLINK = $(CC)
LIBS =
OBJS = main.o airport.o runway.o flight.o
RM = rm -f
# Creating the executable (airport)
airport: $(OBJS)
	$(CCLINK) -o airport $(OBJS) $(LIBS)

# Creating object files using default rules
main.o: main.c airport.h runway.h flight.h ex2.h
airport.o: airport.c airport.h runway.h flight.h ex2.h
runway.o: runway.c runway.h flight.h ex2.h
flight.o: flight.c flight.h ex2.h
# Cleaning old files before new make
clean:
	$(RM) airport screen_test *.o *.bak *~ "#"* core
