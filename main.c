#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "userInput.h"
#include "map.h"
#include "newSleep.h"
#include "laserTank.h"/*MUST PUT AFTER linkedlist.h as the it got include the LinkedList(means LinkedList struct need to be defined FIRST)*/

int main(int argc,char* argv[])
{
    /*mirrorRow and mirrorColumn is just for temporary storing the mirror details in input.txt*/
	int columnSize,playerRow,playerColumn,enemyRow,enemyColumn,i,j,mirrorsNum;
    int stopProgram,mirrorInMap,correctMirrorType;
	char playerDirection,enemyDirection,action,newTankDirection;
    char** map;
    char** copyPtr;
    int* rowSize;
    FILE* inPtr;/*declare a file pointer for input.txt(file to read)*/
    Mirror** mirrors;
    LinkedList* list;
    
    inPtr = NULL;
    list = NULL;
    
    if(argc != 3)/*executablefile input.txt output.txt*/
    {
        printf("Not enough argument.FIRST argument = input text file,SECOND argument = output text file.\n");
    }
    else
    {
    /*instead of doing converting, we do the read file here n save it to the relevant variables*/
    inPtr = fopen(argv[1],"r");
    
    if(inPtr == NULL)
    {
        perror("File does not exist.Could not open the file");
    }
    else
    {
    /*We need to self keep track the size of 2D array as the size of the char pointer is 8 bytes*/
    rowSize = (int*)malloc(sizeof(int));
    
    mirrorInMap = TRUE;/*used to check that if all mirrors are located in map*/
    correctMirrorType = TRUE;/*used to check that all mirrors have correct type(b or f)*/
    
    /*SAVE THE GAME DETAILS INTO RELEVANT VARIABLES*/
    mirrors = convertFile(inPtr,rowSize,&columnSize,&playerRow,&playerColumn,&playerDirection,&enemyRow,&enemyColumn,&enemyDirection,&mirrorsNum,&mirrorInMap,&correctMirrorType);
    for(i = 0 ; i < mirrorsNum ; i++)
    {        
        printf("%d %d %c\n",mirrors[i]->row,mirrors[i]->column,mirrors[i]->type);
    }
    /*REMEMBER TO CLOSE THE FILE AFTER USED*/
    
    /*Before we start, check the rowsize and columnsize for map is in range or not 1st*/
    if((checkMapCoordinate(rowSize) == FALSE) || (checkMapCoordinate(&columnSize) == FALSE))
    {
        printf("Sorry.Program stops as row size and column size(for map) can only be within range 5 and 25\n");
    }
    /*Check whether the player tank coordinate is within map or out of map*/
    else if(tankCoordinate(rowSize,&columnSize,&playerRow,&playerColumn) == FALSE)
    {
        printf("Sorry.Program stops as player coordinate(row:%d) can only be in between 1 and %d(row size for map - 2)\n",playerRow,*rowSize - 2);
        printf("Sorry.Program stops as player coordinate(column:%d) can only be in between 1 and %d(column size for map - 2)\n",playerColumn,columnSize - 2);
    }
    /*Check whether the enemy tank coordinate is within map or out of map*/
    else if(tankCoordinate(rowSize,&columnSize,&enemyRow,&enemyColumn) == FALSE)
    {
        printf("Sorry.Program stops as enemy coordinate(row:%d) can only be be in between 1 and %d(row size for map - 2)\n",enemyRow,*rowSize - 2);
        printf("Sorry.Program stops as enemy coordinate(column:%d) can only be in between 1 and %d(column size for map - 2)\n",enemyColumn,columnSize - 2);
    }
    /*Check the player tank direction and enemy's tank direction is u/d/l/r*/
    else if((checkTankDirection(&playerDirection) == FALSE) || (checkTankDirection(&enemyDirection) == FALSE))
    {
        printf("Sorry.Program stops as direction for player tank and enemy tank can only be u/d/l/r\n");
    }
    /*check whether all Mirrors is within the map or out of map*/
    else if(mirrorInMap == FALSE)
    {
        printf("Sorry.Program stops as ONE OF THE MIRROR's corrdinate does not located in map(Row number MUST in between 1 and %d(row size for map - 2) and column number MUST in between 1 and %d(column size for map - 2\n",*rowSize - 2,columnSize - 2);
    }
    else if(correctMirrorType == FALSE)
    {
        printf("Sorry.Program stops as ONE OF THE MIRROR's corrdinate DOES NOT have CORRECT mirror type.\n");
    }
    else
    {  
    /*create the EMPTY linkedlist now*/
    list = createLinkedList();
        
    /*create dynamic 2D array for map*/
	/*malloc array of char pointers .(Each char pointer will points to the string)*/
    map = (char**)malloc(*rowSize * sizeof(char*));	
	/*malloc pointer to the string(with Size = columnSize + 1)*/
    for(i = 0 ; i < *rowSize ; i++)
    {
        map[i] = (char*)malloc((columnSize + 1) * sizeof(char));/*If using calloc, then u need to use , instead of * */
        /* +1 for null terminator character,'\0'. !!!map does not need + 1 as it is just array of (char) pointers*/
        
        /*fill the map with the space(we cannot print NULL)*/
        for(j = 0 ; j < columnSize ; j++)
        {
            map[i][j] = ' ';
        }
    } 
    
    /*build the wall of the map*/
    for(i = 0 ; i < *rowSize ; i++)
    {        
        /*Don't forget to assign '\0' to the end of the string/char array*/
        map[i][columnSize] = '\0'; 
        
        map[i][0] = '*';/*left part*/
        map[i][columnSize - 1] = '*';/*right part*/  
        for(j = 0 ; j < columnSize ; j++)
        {
            map[0][j] = '*';/*upper part*/
            map[*rowSize - 1][j] = '*';/*bottom part*/
        }
    }
    
    /*build the mirrors in the map*/
    for(i = 0 ; i < mirrorsNum ; i++)
    {
        /*1. CHANGE the mirror to the CORRECT pattern BASED ON the mirror's type*/
        /*CONFIRM you have EITHER b OR f now so...*/
        if(mirrors[i]->type == 'b')
        {
            mirrors[i]->type = '\\';
        }
        else/*mirrors[i]->type == 'f'*/
        {
            mirrors[i]->type = '/';
        }
        
        /*2. SET the mirror in the map*/
        map[mirrors[i]->row][mirrors[i]->column] = mirrors[i]->type;
    }
    
    /* argv[5] = a char pointer(points to a char array)
     *argv[5] = argv[5][0] = first element in argv[5](char array)*/
    
    /*set the direction for both player tank and enemy tank*/
    firstTankDirection(&playerDirection);
    firstTankDirection(&enemyDirection);
    
    /*set the position of the player tank and enemy tank in the map*/
    map[playerRow][playerColumn] = playerDirection;
    map[enemyRow][enemyColumn] = enemyDirection;
    
    /*INSERT THE DATA(FIRST MAP) INTO THE LIST*/
    /*copy = copyMap(map,rowSize,&columnSize);
    copyPtr = &copy;*/
    copyPtr = copyMap(map,rowSize,&columnSize);
    insertLast(list,copyPtr);
    
    stopProgram = FALSE;
    /*stop while only when player/enemy win or loss*/
    while(stopProgram == FALSE)
    {
        /*
         clear
         print map + menu
         input command, BUT HAVEN'T update the map*/
        action = playerInput(map,rowSize);
       
        /*action is either w/s/a/d/f now*/
        switch(action)
        {
        case 'f':
            
            shootLaser(map,rowSize,&columnSize,&playerDirection,&stopProgram,&playerRow,&playerColumn,&enemyRow,&enemyColumn,list,argv[2]);
            /*NOTE THAT I HAVE DONE EVERYTHING IN THE shootLaser()
             - shoot laser
             - change direction if hit mirror(in "Helper" function of shootLaser():nextLaserMovement())
             - WHAT TO DO if laser hit the WALL,MIRROR,TANK AND NOTHING(SPACE)
             - if hit the tank,DECIDE whether player/enemy win
             - WRITE TO THE FILE WHEN THE GAME IS END*/
        break;
        
        /*action is w/s/a/d*/
        case 'w':
        case 's':
        case 'a':
        case 'd':
            /*initialise newTankDirection(need this for if-else below). CANNOT directly update playerDirection as u need to decide to move or change the tank*/
            playerTankDirection(action,&newTankDirection);
           
            /*case 1: If NOT same as previous playerDirection,update the new direction*/
            if(newTankDirection != playerDirection)
            {
                playerDirection = newTankDirection;/*update player Direction*/
                
                map[playerRow][playerColumn] = playerDirection;/*update player tank in map(If DON'T update, your map player tank will not change the direction)*/
                                
        
            } 
            /*case 2: If SAME as previous playerDirection,move tank
             !!!remember to update the column number or row number as well*/
            else
            {
                if(newTankDirection == '<')
                {
                    /*If left side of player tank have nothing,then we can set the new position for player tank*/
                    if(map[playerRow][playerColumn - 1] == ' ')
                    {
                        /*must set previous position to ' ' 1st.Otherwise, will have 2 player tank printed on the map*/
                        map[playerRow][playerColumn] = ' ';
                        playerColumn--;
                        /*update the player direction in map*/
                        map[playerRow][playerColumn] = playerDirection;
                    }
                    /*If left side of player tank is enemy tank/wall border, then do nothing*/
                    else
                    {
                        printf("Cannot replace the wall border/enemy tank/mirror with the player tank!\n");
                    }
                   
                }
                else if(newTankDirection == '>')
                {
                    if(map[playerRow][playerColumn + 1] == ' ')/*ensure right side of the tank is not wall border/enemy tank*/
                    {
                        /*must set previous position to ' '1st.Otherwise, will have 2 player tank printed on the map*/
                        map[playerRow][playerColumn] = ' ';
                        playerColumn++;
                        /*update the player direction in map*/
                        map[playerRow][playerColumn] = playerDirection;
                    }
                    /*If right side of player tank is enemy tank/wall border, then do nothing*/
                    else
                    {
                        printf("Cannot replace the wall border/enemy tank/mirror with the player tank!\n");
                    }
                 
                }
                else if(newTankDirection == '^')
                {
                    if(map[playerRow - 1][playerColumn] == ' ')/*ensure there is not wall border/enemy tank on top of the tank*/
                    {
                        /*must set previous position to ' '1st.Otherwise, will have 2 player tank printed on the map*/
                        map[playerRow][playerColumn] = ' ';
                        playerRow--;
                        /*update the player direction in map*/
                        map[playerRow][playerColumn] = playerDirection;
                    }
                    /*If top of player tank is enemy tank/wall border, then do nothing*/
                    else
                    {
                        printf("Cannot replace the wall border/enemy tank/mirror with the player tank!\n");
                    }
                  
                }
                else
                {
                    if(map[playerRow + 1][playerColumn] == ' ')/*ensure there is not wall border/enemy tank below the tank*/
                    {
                        /*must set previous position to ' '1st.Otherwise, will have 2 player tank printed on the map*/
                        map[playerRow][playerColumn] = ' ';
                        playerRow++;
                        /*update the player direction in map*/
                        map[playerRow][playerColumn] = playerDirection;
                    }
                    /*If bottom of player tank is enemy tank/wall border, then do nothing*/
                    else
                    {
                        printf("Cannot replace the wall border/enemy tank/mirror with the player tank!\n");
                    }
                }
            }
            
            /*INSERT THE MAP(AFTER CHANGING PLAYER DIRECTION/POSITION) INTO THE LIST*/
            copyPtr = copyMap(map,rowSize,&columnSize);
            insertLast(list,copyPtr);
        break;
        
        case 'l':
            printLinkedList(list,argv[2],&processMap,rowSize);
        break;
        }   
        
        /*CHECK IF ENEMY WIN...U GO TO LOOK AT THE PREVIOUS ASSIGNMENT TO SEE HOW I DO IT*/
        /*basically check if PLAYER tank IN FRONT of the ENEMY tank*/
        if(playerInFrontEnemy(map,&enemyDirection,&enemyRow,&enemyColumn,rowSize,&columnSize,&playerRow,&playerColumn) == TRUE)/*means PLAYER tank IN FRONT of the ENEMY tank*/
        {
            /*then we need to shoot the player tank*/
            shootPlayer(map,rowSize,&columnSize,&playerDirection,&playerRow,&playerColumn,&enemyDirection,&enemyRow,&enemyColumn,list,argv[2]);
            
            /*END THE GAME*/
            stopProgram = TRUE;
            
            /* DON'T FORGET TO INSERT THIS MAP INTO THE LINKED LIST
             * BUT I HAVE MAKE THIS IN THE shootPlayer() N WRITE IT TO THE FILE*/
        }
    }
    
    
    /*free all the allocated memory in heap*/
    for(i = 0 ; i < *rowSize ; i++)
    {
        free(map[i]);
    }    
    free(map);
    freeLinkedList(list,&freeData,rowSize);
    }
    free(rowSize);
    for(i = 0 ; i < mirrorsNum ; i++)
    {
        free(mirrors[i]);
    }    
	free(mirrors);
   
    /*close the file stream*/
    fclose(inPtr);
    }
    }
	return 0;
}


