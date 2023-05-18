/*print.c contains all displaying function such as displaying map,laser*/
#include <stdio.h>
#include <stdlib.h>/*for malloc in processMap()*/
#include "map.h"
#include "newSleep.h"

/*to print the map*/
void printMap(char** map,int* rowSize)
{
    int i;    
	for(i = 0 ; i < *rowSize ; i++)
    {
        printf("%s\n",map[i]);
    }
}

void redPrint(char* sentence)
{
  printf("\033[0;31m");	/* change font color into red color for the oncoming printf function */
  printf("%s", sentence);
  printf("\033[0m");		/* change font color back to default color (basically a reset button)*/
}

/*"Helper" function for writing the linked list into the output text file*/
/*void* data = char** map at here*/
void processMap(FILE* outPtr,void* data,int* rowSize)
{
    int i;
    /*1. COPY the map*/
    
    char** map = (char**)data;
    for(i = 0 ; i < *rowSize;i++)
    {
        /* fputs() writes a string, plus a new line ('\n').
        Takes two parameters: char* and FILE*.*/
        /*2. write A line of the map into the file*/
        fputs(map[i],outPtr);
        /*3. then write a newline*/
        fprintf(outPtr,"\n");
    }
    /*4. after done, write this into the file(for separating each map)*/
    fputs("---------------------------------\n",outPtr);
}


