CFLAGS = -g -Wall -ansi -pedantic -Werror
OBJS = main.o laserTank.o userInput.o map.o newSleep.o linkedlist.o
CC = gcc
EXEC = laserTank

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)
	
main.o: main.c linkedlist.c laserTank.c map.c userInput.c newSleep.c linkedlist.h laserTank.h map.h userInput.h newSleep.h  
	$(CC) $(CFLAGS) -c main.c
	
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c
	
laserTank.o:linkedlist.c laserTank.c userInput.c map.c newSleep.c linkedlist.h laserTank.h userInput.h map.h newSleep.h
	$(CC) $(CFLAGS) -c laserTank.c
	
map.o: map.c newSleep.c map.h newSleep.h
	$(CC) $(CFLAGS) -c map.c

userInput.o: userInput.c map.c newSleep.c userInput.h map.h newSleep.h
	$(CC) $(CFLAGS) -c userInput.c
	
newSleep.o: newSleep.c newSleep.h
	$(CC) $(CFLAGS) -c newSleep.c
	
clean:
	rm -f $(OBJS) $(EXEC)
