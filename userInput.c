/*This file is to check the user input and also have all the methods for taking user input.*/

#include <stdio.h>
#include <stdlib.h>
#include "userInput.h"
#include "map.h"
#include "newSleep.h"

/*********************************
 * START: CHECKING USER INPUT
 *********************************/
/*This is for checkMapCoordinate(),tankCoordinate(),checkMirrorCoordinate() to check the range*/
int checkRange(int low,int high,int* rowOrColumn)
{
    int boolean;
  
    if(*rowOrColumn >= low && *rowOrColumn <= high)
    {
        boolean = TRUE;
    }
    else
    {
        boolean = FALSE;
    }
    return boolean;
}

/*to check the rowsize and columnsize for map is in range or not*/
int checkMapCoordinate(int* size)
{
    int boolean;
    if(checkRange(5,25,size) == TRUE)
    {
        boolean = TRUE;        
    }
    else
    {
        boolean = FALSE;
    }
    return boolean;
}

/*to check the tank coordinate entered in the commnad line is within the map*/
int tankCoordinate(int* rowSize,int* columnSize,int* tankRow,int* tankColumn)
{
    int boolean;
    /*can only be 1 to (rowSize - 2) as we don't want to hit the wall of map*/
    if(checkRange(1,*rowSize - 2,tankRow) && checkRange(1,*columnSize - 2,tankColumn))
    {
        boolean = TRUE;
    }
    else
    {
        boolean = FALSE;
    }
    return boolean;
}

/*to check the tank direction(entered in the terminal command line)*/
int checkTankDirection(char* direction)
{
    int boolean;
    if(*direction == 'u' || *direction == 'd' || *direction == 'l' || *direction == 'r')
    {
        boolean = TRUE;
    }
    else
    {
        boolean = FALSE;
    }
    return boolean;
}

/*static as this method is used by playerInput() only
 This method is to validate the user input(can only enter w/s/a/d/f)*/
static int checkPlayerInput(char* input)
{
    int boolean;
    if(*input == 'w' || *input == 's' || *input == 'a' || *input == 'd' || *input == 'f' || *input == 'l')
    {
        boolean = TRUE;
    }
    else
    {
        boolean = FALSE;
        printf("Please re-enter the input. ONLY(w/s/a/d/f/l)\n");
    }
    return boolean;
}
/*********************************
 * END: CHECKING USER INPUT
 *********************************/

/*to print the map + displaying menu for player, w/s/a/d/f + get the user input*/
char playerInput(char** map,int* rowSize)
{
    int boolean;
    char input = ' '; 
    boolean = FALSE;
    while(boolean == FALSE)
    {
        /*newSleep(0.5); 1st so that we can see what is the user input "action: "*/
        newSleep(0.5);
        /*clear the screen actually is needed for display the latest user input(clear the previous one)*/
        system("clear");
        
        printMap(map,rowSize);
        printf("Please enter (w/s/a/d/f/l) ONLY\n");
        printf("w to go/face up\n");
        printf("s to go/face down\n");
        printf("a to go/face left\n");
        printf("d to go/face right\n");
        printf("f to shoot laser\n");
        printf("l to save the Log\n");
        scanf(" %c",&input);
        printf("action: %c\n",input);
        
        
        /*check the input*/
        boolean = checkPlayerInput(&input);
    
    }
    
return input;
}



