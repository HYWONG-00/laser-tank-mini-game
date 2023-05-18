#include <stdio.h>
#include <stdlib.h>
#include <string.h>/*for strcpy used in copyMap*/
#include "linkedlist.h"
#include "userInput.h"
#include "map.h"
#include "newSleep.h"
#include "laserTank.h"


/*SAVE THE GAME DETAILS INTO RELEVANT VARIABLES*/
Mirror** convertFile(FILE* inPtr,int* rowSize,int* columnSize,int* playerRow,int* playerColumn,char* playerDirection,int* enemyRow,int* enemyColumn,char* enemyDirection,int* mirrorsNum,int* mirrorInMap,int* correctMirrorType)
{
    Mirror** mirrors;
    int mirrorRow,mirrorColumn,i;
    char mirrorType;
    
    /*read the FIRST line n save details into ADDRESS of the variables*/
    fscanf(inPtr,"%d %d",rowSize,columnSize);
    printf("%d %d\n",*rowSize,*columnSize);
    /*read the SECOND line*/
    fscanf(inPtr,"%d %d %c",playerRow,playerColumn,playerDirection);
    printf("%d %d %c\n",*playerRow,*playerColumn,*playerDirection);
    /*read the THIRD line*/
    fscanf(inPtr,"%d %d %c",enemyRow,enemyColumn,enemyDirection);
    printf("%d %d %c\n",*enemyRow,*enemyColumn,*enemyDirection);
    
    /*COUNT the mirror number in the file FIRST before creating the Mirror array(to set the arraySize) 2.We can set looping times for creating malloc mirror*/
    *mirrorsNum = 0;/*used to count the mirror in the file*/
    while(fscanf(inPtr,"%d %d %c",&mirrorRow,&mirrorColumn,&mirrorType) != EOF)
    {
        *mirrorsNum = *mirrorsNum + 1;
    }
    printf("%d\n",*mirrorsNum);
    
    /*reset the file pointer back to the start of the file*/
    rewind(inPtr); /*==  fseek(f, 0, SEEK_SET)*/
    
    /*Just re-initialise the variables(for convinience,learn how to use fseek())*/
    fscanf(inPtr,"%d %d",rowSize,columnSize);/*fscanf(inPtr,"%d %d",rowSize,&columnSize);*/
    fscanf(inPtr,"%d %d %c",playerRow,playerColumn,playerDirection);/*fscanf(inPtr,"%d %d %c",&playerRow,&playerColumn,&playerDirection);*/
    fscanf(inPtr,"%d %d %c",enemyRow,enemyColumn,enemyDirection);/*fscanf(inPtr,"%d %d %c",&enemyRow,&enemyColumn,&enemyDirection);*/
    
    
    /*create the array of Mirrors/Mirror struct (with mirrorsNum(array size))*/
    mirrors = (Mirror**)malloc(*mirrorsNum * sizeof(Mirror*));
    for(i = 0 ; i < *mirrorsNum ; i++)
    {        
        /*create Mirror struct in each element*/
        mirrors[i] = (Mirror*)malloc(sizeof(Mirror));
        
        /*read the NEXT line*/
        fscanf(inPtr,"%d %d %c",&mirrorRow,&mirrorColumn,&mirrorType);
        printf("%d %d %c\n",mirrorRow,mirrorColumn,mirrorType);
        
        /*CHECK the mirrors' coordinates(ensure it is in map boundary)*/
        if(checkRange(1,*rowSize - 2,&mirrorRow) == FALSE || checkRange(1,*columnSize - 2,&mirrorColumn) == FALSE)
        {
            *mirrorInMap = FALSE;
        }
        
        /*CHECK the mirrors' type(ensure all mirrors have CORRECT type)*/
        if(mirrorType != 'b' && mirrorType != 'f')
        {
            *correctMirrorType = FALSE;
        }
        
        /*FILL IN THE CONTENT OF THE struct(Mirror)*/
        mirrors[i]->row = mirrorRow;
        mirrors[i]->column = mirrorColumn;
        mirrors[i]->type = mirrorType;
        printf("%d %d %c\n",mirrors[i]->row,mirrors[i]->column,mirrors[i]->type);
    }
    return mirrors;
}

/*to set the tank direction based on the terminal command line,./laserTank 10 20 1 1 r 5 10 u*/
void firstTankDirection(char* tank)
{
    /*CHANGE THE RELEVANT DIRECTION BASED ON THE tank's direction*/
	switch(*tank)
	{
		case 'u':
			*tank = '^';
		break;
		
		case 'd':
			*tank = 'v';
		break;
		
		case 'l':
			*tank = '<';
		break;
		
		case 'r':
			*tank = '>';
		break;
	}
}

/*set the tank direction based on the player input*/
void playerTankDirection(char letter,char* tank)
{
	switch(letter)
	{
		case 'w':
			*tank = '^';
		break;
		
		case 's':
			*tank = 'v';
		break;
		
		case 'a':
			*tank = '<';
		break;
		
		case 'd':
			*tank = '>';
		break;
	}
}

/************************************************************************
 * START: all method used for enemy tank to shoot the laser(CHECK + SHOOT,IF YES)
 **********************************************************************/
/*FOR ENEMY TANK*/
/*to CHECK if the player tank located in front of the enemy tank*/
int playerInFrontEnemy(char** map,char* enemyDirection,int* enemyRow,int* enemyColumn,int* rowSize,int* columnSize,int* playerRow,int* playerColumn)
{
    int inFront;
    inFront = FALSE;
    switch(*enemyDirection)
    {
        case '>':
            /*If player tank and enemy tank ARE IN THE SAME ROW*/
            if(*enemyRow == *playerRow)
            {
                /*N the player tank are at the RIGHT SIDE of the enemy tank*/
                if(*playerColumn > *enemyColumn)
                {
                    inFront = TRUE;
                }
            }
        break;
        
        case '<':
            /*If player tank and enemy tank ARE IN THE SAME ROW*/
            if(*enemyRow == *playerRow)
            {
                /*N the player tank are at the LEFT SIDE of the enemy tank*/
                if(*playerColumn < *enemyColumn)
                {
                    inFront = TRUE;
                }
            }
        break;
        
        case 'v':
            /*If player tank and enemy tank ARE IN THE SAME COLUMN*/
            if(*enemyColumn == *playerColumn)
            {
                /*N the player tank are at the DOWN SIDE of the enemy tank*/
                if(*playerRow > *enemyRow)
                {
                    inFront = TRUE;
                }
            }
        break;
        
        case '^':
            /*If player tank and enemy tank ARE IN THE SAME COLUMN*/
            if(*enemyColumn == *playerColumn)
            {
                /*N the player tank are at the UPPER SIDE of the enemy tank*/
                if(*playerRow < *enemyRow)
                {
                    inFront = TRUE;
                }
            }
        break;
    }
    
return inFront;
}

/*FOR ENEMY SHOOT PLAYER*/
void shootPlayer(char** map,int* rowSize,int* columnSize,char* playerDirection,int* playerRow,int* playerColumn,char* enemyDirection,int* enemyRow,int* enemyColumn,LinkedList* list,char* filename)
{
    char** copyPtr;
    char laser;
    int i,j,k,start,end;
    
    switch(*enemyDirection)
    {
        case '>':
            laser = '-';
            start = *enemyColumn + 1;/*start from the space in front of the enemy tank*/
            end = *playerColumn;/*stop at one space b4 the player tank*/
         
            for(k = start ; k < end ; k++)
            {
                newSleep(0.5);
                system("clear");
                
                /*set the NEW laser position*/
                map[*enemyRow][k] = laser;
                
                /*REMEMBER TO SAVE IT INTO THE LINKEDLIST (BEFORE U SET THE laser to ' ' && INSIDE THE LOOP)*/
                copyPtr = copyMap(map,rowSize,columnSize);
                insertLast(list,copyPtr);
                
                /*print the map with the laser*/
                for(i = 0 ; i < *rowSize ; i++)
                {
                    for(j = 0 ; j < *columnSize ; j++)
                    {
                        if(map[i][j] == '|' || map[i][j] == '-')
                        {
                            redPrint(&laser);
                        }
                        else
                        {
                            printf("%c",map[i][j]);
                        }
                    }
                    printf("\n");
                }
                
                /*set the PREVIOUS laser position*/
                map[*enemyRow][k] = ' ';
            }
        break;
        
        case '<':
            laser = '-';
            start = *enemyColumn - 1;/*start from the space in front of the enemy tank*/
            end = *playerColumn + 1;/*stop at one space b4 hit the player tank*/
            
            /*MUST k-- NOT k++. As the laser move to the left(NOT to the right). If u don't understand, you can try k = end ; k < start  ; k++. U observe how the laser move*/
            for(k = start ; k >= end ; k--)
            {
                newSleep(0.5);
                system("clear");
                    
                /*set the NEW laser position*/
                map[*enemyRow][k] = laser;           
                
                /*REMEMBER TO SAVE IT INTO THE LINKEDLIST (BEFORE U SET THE laser to ' ' && INSIDE THE LOOP)*/
                copyPtr = copyMap(map,rowSize,columnSize);
                insertLast(list,copyPtr);
                
                /*print the map with the laser*/
                for(i = 0 ; i < *rowSize ; i++)
                {
                    for(j = 0 ; j < *columnSize ; j++)
                    {
                        if(map[i][j] == '|' || map[i][j] == '-')
                        {
                            redPrint(&laser);
                        }
                        else
                        {
                            printf("%c",map[i][j]);
                        }
                    }
                    printf("\n");
                }
                
                /*set the PREVIOUS laser position*/
                map[*enemyRow][k] = ' ';
            }       
        break;
        
        case 'v':
            laser = '|';
            start = *enemyRow + 1;
            end = *playerRow;/*stop at one space b4 hit the player tank*/
            
            for(k = start ; k < end ; k++)
            {
                newSleep(0.5);
                system("clear");
                
                /*set the NEW laser position*/
                map[k][*enemyColumn] = laser;
                
                /*REMEMBER TO SAVE IT INTO THE LINKEDLIST (BEFORE U SET THE laser to ' ' && INSIDE THE LOOP)*/
                copyPtr = copyMap(map,rowSize,columnSize);
                insertLast(list,copyPtr);
                
                /*print the map with the laser*/
                for(i = 0 ; i < *rowSize ; i++)
                {
                    for(j = 0 ; j < *columnSize ; j++)
                    {
                        if(map[i][j] == '|' || map[i][j] == '-')
                        {
                            redPrint(&laser);
                        }
                        else
                        {
                            printf("%c",map[i][j]);
                        }
                    }
                    printf("\n");
                }
                
                /*set the PREVIOUS laser position*/
                map[k][*enemyColumn] = ' ';
            }
        break;
        
        case '^':
            laser = '|';
            start = *enemyRow - 1;
            end = *playerRow + 1;
            
            for(k = start ; k >= end ; k--)
            {
                newSleep(0.5);
                system("clear");
                
                /*set the NEW laser position*/
                map[k][*enemyColumn] = laser;
                
                /*REMEMBER TO SAVE IT INTO THE LINKEDLIST (BEFORE U SET THE laser to ' ' && INSIDE THE LOOP)*/
                copyPtr = copyMap(map,rowSize,columnSize);
                insertLast(list,copyPtr);
                
                /*print the map with the laser*/
                for(i = 0 ; i < *rowSize ; i++)
                {
                    for(j = 0 ; j < *columnSize ; j++)
                    {
                        if(map[i][j] == '|' || map[i][j] == '-')
                        {
                            redPrint(&laser);
                        }
                        else
                        {
                            printf("%c",map[i][j]);
                        }
                    }
                    printf("\n");
                }
                
                /*set the PREVIOUS laser position*/
                map[k][*enemyColumn] = ' ';
            }
        break;
    }

    /*LAST MAP:*/
    newSleep(0.5);
    system("clear");
    
    /*player tank turns X*/
    map[*playerRow][*playerColumn] = 'X';
    printMap(map,rowSize);
    printf("You(player) lose!\n");
    
    /*I think can just insertLast at here*/
    /*DON'T FORGET TO INSERT THE LAST MAP(PLAYER/ENEMY GET KILLED) INTO THE LIST*/
    copyPtr = copyMap(map,rowSize,columnSize);
    insertLast(list,copyPtr);
    /*SINCE THE GAME IS END NOW,WE NEED TO WRITE THE LINKEDLIST(WHOLE GAME) INTO THE FILE*/
    printLinkedList(list,filename,&processMap,rowSize);
    
}
/************************************************************************
 * END: all method used for enemy tank to shoot the laser(CHECK + SHOOT,IF YES)
 **********************************************************************/

/************************************************************************
 * START: all method used for player tank shooting laser(option: f)
 **********************************************************************/
/*FOR PLAYER ONLY*/
/*for player tank to set the laser
 HAVE DONE: check + decide what to do if laser hit MIRROR/WALL/TANK when it is moving(HAVE CHECKED for ALL PLAYER'S DIRECTION)*/
/*actually tankRow n tankColumn == playerRow n playerColumn*/
void shootLaser(char** map,int* rowSize,int* columnSize,char* playerDirection,int* stopProgram,int* playerRow,int* playerColumn,int* enemyRow,int* enemyColumn,LinkedList* list,char* filename)
{
    char** copyPtr;
    char laser;
    int i,j;
    int stopLaser,laserRow,laserColumn;
    int moveLeft,moveRight,moveUp,moveDown;
    stopLaser = FALSE;
    /*SIMPLY INITIALISE THE laser variable.Otherwise,get error: Use of uninitialised value of size 8*/
    laser = '|'; 
    
    /*initialise the laser's direction + coordinate(initialise at the tank position,BUT I DOES NOT SET IT INTO THE MAP)*/
    switch(*playerDirection)
    {
        
        case '>':
            laser = '-';
            /*laserRow = *tankRow;
            laserColumn = *tankColumn + 1;*/
            /*THIS IS NOT THE FINALISED LASER POSITION FOR THE FIRST LASER.AS WE NEED TO SEE THAT WHETHER WE HAVE TANK/MIRROR/WALL AT THE FIRST LASER POSITION BEFORE WE SET IT INTO THE MAP*/
            laserRow = *playerRow;
            laserColumn = *playerColumn;
            moveLeft = FALSE;
            moveRight = TRUE;
            moveUp = FALSE;
            moveDown = FALSE;
        break;
        
        case '<':
            laser = '-';
            /*laserRow = *tankRow;
            laserColumn = *tankColumn - 1;*/
            /*THIS IS NOT THE FINALISED LASER POSITION FOR THE FIRST LASER.AS WE NEED TO SEE THAT WHETHER WE HAVE TANK/MIRROR/WALL AT THE FIRST LASER POSITION BEFORE WE SET IT INTO THE MAP*/
            laserRow = *playerRow;
            laserColumn = *playerColumn;
            moveLeft = TRUE;
            moveRight = FALSE;
            moveUp = FALSE;
            moveDown = FALSE;
        break;
        
        case '^':
            laser = '|';
            /*laserRow = *tankRow - 1;
            laserColumn = *tankColumn;*/
            /*THIS IS NOT THE FINALISED LASER POSITION FOR THE FIRST LASER.AS WE NEED TO SEE THAT WHETHER WE HAVE TANK/MIRROR/WALL AT THE FIRST LASER POSITION BEFORE WE SET IT INTO THE MAP*/
            laserRow = *playerRow;
            laserColumn = *playerColumn;
            moveLeft = FALSE;
            moveRight = FALSE;
            moveUp = TRUE;
            moveDown = FALSE;
        break;
        
        case 'v':
            laser = '|';
            /*laserRow = *tankRow + 1;
            laserColumn = *tankColumn;*/
            /*THIS IS NOT THE FINALISED LASER POSITION FOR THE FIRST LASER.AS WE NEED TO SEE THAT WHETHER WE HAVE TANK/MIRROR/WALL AT THE FIRST LASER POSITION BEFORE WE SET IT INTO THE MAP*/
            laserRow = *playerRow;
            laserColumn = *playerColumn;
            moveLeft = FALSE;
            moveRight = FALSE;
            moveUp = FALSE;
            moveDown = TRUE;
        break;
    }
    
    /*map[laserRow][laserColumn] = laser;*/
    
    while(stopLaser == FALSE)/*loop until I say stopLaser = TRUE;*/
    {
        /*DECIDE N SET laser position.
         WHY WE NEED TO DO THIS FIRST? ESPECIALLY IMPORTANT IF THERE IS A MIRROR IN FRONT OF THE TANK
         eg:
         v - tank
         / - mirror*/
        setLaser(map,&laser,&laserRow,&laserColumn,&stopLaser,&moveLeft,&moveRight,&moveUp,&moveDown,stopProgram);
        
        newSleep(0.5);
        system("clear");
        
        /*THIS GOES AFTER setLaser() as I need to DECIDE WHERE TO SET THE (FIRST) LASER POSITION FIRST IN THE MAP FIRST */
        /*to print the whole map(together with the red laser)*/
        for(i = 0 ; i < *rowSize ; i++)
        {
            for(j = 0 ; j < *columnSize ; j++)
            {
                if(map[i][j] == '|' || map[i][j] == '-')
                {
                    redPrint(&laser);
                }
                else
                {
                    printf("%c",map[i][j]);
                }
            }
            printf("\n");
        }
        
        /*I think can just insertLast at here*/
        /*THIS GOES AFTER setLaser() as I need to DECIDE WHERE TO SET THE (FIRST) LASER POSITION FIRST IN THE MAP FIRST.(OTHERWISE,I'll SAVE the WRONG map)*/
        copyPtr = copyMap(map,rowSize,columnSize);
        insertLast(list,copyPtr);
        
        /*I DO INSIDE HERE AS I NEED laserRow n laserColumn number which I dun hv in main function*/
        /*IF AFTER setLaser(), n it says stopProgram = TRUE,this means we hit the tank NOW. BUT we don't know we hit the player/enemy. N that is WHY i need the coordinate of the player tank and enemy tank(used to CHECK I hit which tank n how to stop)*/
        if(*stopProgram == TRUE)/*means laser HIT the (PLAYER/ENEMY)TANK*/
        {
            /*NEED TO SET THE LASER TO ' '.Otherwise, will have laser n X appear in the same map.
             - WILL BE DONE IN THE setLaser*/
            
            /*This is used to display the LAST MAP(player/enemy tank get killed)*/
            newSleep(0.5);
            system("clear");
            
            /*If laser hit the PLAYER tank*/
            if(laserRow == *playerRow && laserColumn == *playerColumn)
            {
                /*player tank turns X*/
                map[*playerRow][*playerColumn] = 'X';
                printMap(map,rowSize);
                printf("You(player) lose!\n");
            }
            /*If laser hit the ENEMY tank*/
            else/*laserRow == *enemyRow && laserColumn == *enemyColumn*/
            {
                /*enemy tank turns X*/
                map[*enemyRow][*enemyColumn] = 'X';
                printMap(map,rowSize);
                printf("You(player) win!\n");
            }
            
        /*I think can just insertLast at here*/
        /*DON'T FORGET TO INSERT THE LAST MAP(PLAYER/ENEMY GET KILLED) INTO THE LIST*/
        copyPtr = copyMap(map,rowSize,columnSize);
        insertLast(list,copyPtr);
        /*SINCE THE GAME IS END NOW,WE NEED TO WRITE THE LINKEDLIST(WHOLE GAME) INTO THE FILE*/
        printLinkedList(list,filename,&processMap,rowSize);
        }
    }
}

/*WHAT SHOULD MY NEXT LASER MOVEMENT BE LIKE?(ask this in each of the move(Left/Right/Up/Down))
 If the thing in front of the laser 
 has NOTHING: then JUST MOVE FORWARD
 is WALL: then JUST STOP LASER LOOPING
 is MIRROR: then laser will CHANGE DIRECTION(call newLaserPosition())
 is TANK: THEN HIT IT and END THE GAME */
/*This function will SET the position(direction + coordinate) of the laser*/
void setLaser(char** map,char* laser,int* laserRow,int* laserColumn,int* stopLaser,int* moveLeft,int* moveRight,int* moveUp,int* moveDown,int* stopProgram)
{
    /*NEED TO CHECK THIS BEFORE U SET IT AS SPACE. AS IT IS POSSIBLE THAT
     v <- player tank
     < <- enemy tank.If like this is that you r going to make player tank to ' '? */
    if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
    {
        map[*laserRow][*laserColumn] = ' ';
    }
    
    if(*moveLeft == TRUE)
    {
        /*If there is SPACE in front when moving*/
        if(map[*laserRow][*laserColumn - 1] == ' ')
        {
            /*THEN JUST MOVE THE LASER FORWARD ONLY(JUST SET the laser position nia)
             1. set the position back to ' ' FIRST.Otherwise you will have ------*/
            
             /*2. update the laserColumn FIRST(laserRow remains unchanged)*/
             *laserColumn = *laserColumn - 1;
             /*3. set the new position for the laser in the map*/
             map[*laserRow][*laserColumn] = *laser; 
        }
        /*If there is WALL in front when moving*/
        else if(map[*laserRow][*laserColumn - 1] == '*')
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*set the position back to ' ' FIRST.Otherwise you will have '-' on map when u ask for USER INPUT(after u exit the shootLaser())
             - have done this at top there.*/
        }
        /*If there is MIRROR in front when moving*/
        else if(map[*laserRow][*laserColumn - 1] == '\\' || map[*laserRow][*laserColumn - 1] == '/')
        {
            /*1. DON'T FORGET to set this location as ' ' BEFORE u update the NEW laser position. Otherwise,you will have MULTIPLE laser appeared on the map 
            if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
            {
                map[*laserRow][*laserColumn] = ' ';
            }*/
            
            /*THEN CHANGE THE DIRECTION*/
            newLaserPosition(laser,laserRow,laserColumn,&(map[*laserRow][*laserColumn - 1]),moveLeft,moveRight,moveUp,moveDown);
                
            /*3. set the NEW laser position AFTER updating the laser details n movement(boolean) */
            /*!!!AFTER REFLECTING THE LASER,WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN  SO...*/
            if(map[*laserRow][*laserColumn] != ' ')
            {
                /*I think we need to make a recursive call at here but i also dk i do correct or not*/
                /*SINCE WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN,N WE HAVE DONE THIS IN THE setLaser(). Then, JUST call this function again...*/
                setLaser(map,laser,laserRow,laserColumn,stopLaser,moveLeft,moveRight,moveUp,moveDown,stopProgram);
            }
            else/*If it is a space again,then we just set it*/
            {
                map[*laserRow][*laserColumn] = *laser; 
            }
        }
        /*If there is TANK in front when moving*/
        else
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*DON'T FORGET TO SET THE LASER TO ' '.Otherwise, will have laser n X appear in the same map.
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
            
            /*UPDATE THE laser coordinate,SET THE LASER COORDINATE TO THE TANK'S COORDINATE
             WHY WE NEED TO DO THIS? As after we return to the shootLaser(),we need to DECIDE WHICH TANK GET HITTED.So, this laser coordinate HAVE TO BE UPDATED/MOVE ONE STEP FORWARD(!!BUT I DOES NOT SET THIS INTO THE MAP YA(How can I replace the tank position?))*/
            *laserColumn = *laserColumn - 1;
            
            /*END THE GAME*/
            *stopProgram = TRUE;
        }
    }
    else if(*moveRight == TRUE)
    {
        /*If there is SPACE in front when moving*/
        if(map[*laserRow][*laserColumn + 1] == ' ')
        {
            /*THEN JUST MOVE THE LASER FORWARD ONLY(JUST SET the laser position nia)
             1. set the position back to ' ' FIRST.Otherwise you will have ------
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
            
             /*2. update the laserColumn FIRST(laserRow remains unchanged)*/
             *laserColumn = *laserColumn + 1;
             /*3. set the new position for the laser in the map*/
             map[*laserRow][*laserColumn] = *laser; 
        }
        /*If there is WALL in front when moving*/
        else if(map[*laserRow][*laserColumn + 1] == '*')
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*set the position back to ' ' FIRST.Otherwise you will have '-' on map when u ask for USER INPUT(after u exit the shootLaser())
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
            
        }
        /*If there is MIRROR in front when moving*/
        else if(map[*laserRow][*laserColumn + 1] == '\\' || map[*laserRow][*laserColumn + 1] == '/')
        {
            /*1. DON'T FORGET to set this location as ' ' BEFORE u update the NEW laser position. Otherwise,you will have MULTIPLE laser appeared on the map
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/           
            
            /*THEN CHANGE THE DIRECTION*/
            newLaserPosition(laser,laserRow,laserColumn,&(map[*laserRow][*laserColumn + 1]),moveLeft,moveRight,moveUp,moveDown);            
                
            /*3. set the NEW laser position AFTER updating the laser details n movement(boolean) */
            /*!!!AFTER REFLECTING THE LASER,WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN  SO...*/
            if(map[*laserRow][*laserColumn] != ' ')
            {
                /*I think we need to make a recursive call at here but i also dk i do correct or not*/
                /*SINCE WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN,N WE HAVE DONE THIS IN THE setLaser(). Then, JUST call this function again...*/
                setLaser(map,laser,laserRow,laserColumn,stopLaser,moveLeft,moveRight,moveUp,moveDown,stopProgram);
            }
            else/*If it is a space again,then we just set it*/
            {
                map[*laserRow][*laserColumn] = *laser; 
            }
        }
        /*If there is TANK in front when moving*/
        else
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*DON'T FORGET TO SET THE LASER TO ' '.Otherwise, will have laser n X appear in the same map.
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
           
            /*UPDATE THE laser coordinate,SET THE LASER COORDINATE TO THE TANK'S COORDINATE
             WHY WE NEED TO DO THIS? As after we return to the shootLaser(),we need to DECIDE WHICH TANK GET HITTED.So, this laser coordinate HAVE TO BE UPDATED/MOVE ONE STEP FORWARD(!!BUT I DOES NOT SET THIS INTO THE MAP YA(How can I replace the tank position?))*/
            *laserColumn = *laserColumn + 1;
            
            /*END THE GAME*/
            *stopProgram = TRUE;
        }
    }
    else if(*moveUp == TRUE)
    {
        /*If there is SPACE in front when moving*/
        if(map[*laserRow - 1][*laserColumn] == ' ')
        {
            /*THEN JUST MOVE THE LASER FORWARD ONLY(JUST SET the laser position nia)
             1. set the position back to ' ' FIRST.Otherwise you will have ------
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
             
             /*2. update the laserRow FIRST(laserColumn remains unchanged)*/
             *laserRow = *laserRow - 1;
             /*3. set the new position for the laser in the map*/
             map[*laserRow][*laserColumn] = *laser; 
        }
        /*If there is WALL in front when moving*/
        else if(map[*laserRow - 1][*laserColumn] == '*')
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*set the position back to ' ' FIRST.Otherwise you will have '-' on map when u ask for USER INPUT(after u exit the shootLaser())
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
           
        }
        /*If there is MIRROR in front when moving*/
        else if(map[*laserRow - 1][*laserColumn] == '\\' || map[*laserRow - 1][*laserColumn] == '/')
        {
            /*1. DON'T FORGET to set this location as ' ' BEFORE u update the NEW laser position. Otherwise,you   will have MULTIPLE laser
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
            
            /*THEN CHANGE THE DIRECTION*/
            newLaserPosition(laser,laserRow,laserColumn,&(map[*laserRow - 1][*laserColumn]),moveLeft,moveRight,moveUp,moveDown);
            
            /*3. set the NEW laser position AFTER updating the laser details n movement(boolean) */
            /*!!!AFTER REFLECTING THE LASER,WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN  SO...*/
            if(map[*laserRow][*laserColumn] != ' ')
            {
                /*I think we need to make a recursive call at here but i also dk i do correct or not*/
                /*SINCE WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN,N WE HAVE DONE THIS IN THE setLaser(). Then, JUST call this function again...*/
                setLaser(map,laser,laserRow,laserColumn,stopLaser,moveLeft,moveRight,moveUp,moveDown,stopProgram);
            }
            else/*If it is a space again,then we just set it*/
            {
                map[*laserRow][*laserColumn] = *laser; 
            }
        }
        /*If there is TANK in front when moving*/
        else
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*DON'T FORGET TO SET THE LASER TO ' '.Otherwise, will have laser n X appear in the same map.
             -HAVE DONE THIS AT TOP OF THIS FUNCTION*/
           
            
            /*UPDATE THE laser coordinate,SET THE LASER COORDINATE TO THE TANK'S COORDINATE
             WHY WE NEED TO DO THIS? As after we return to the shootLaser(),we need to DECIDE WHICH TANK GET HITTED.So, this laser coordinate HAVE TO BE UPDATED/MOVE ONE STEP FORWARD(!!BUT I DOES NOT SET THIS INTO THE MAP YA(How can I replace the tank position?))*/
            *laserRow = *laserRow - 1;
            
            /*END THE GAME*/
            *stopProgram = TRUE;
        }
        /*If there is TANK in front when moving*/
    }
    else/* *moveDown == TRUE*/
    {     
        
       /*If there is SPACE in front when moving*/
        if(map[*laserRow + 1][*laserColumn] == ' ')
        {
            /*THEN JUST MOVE THE LASER FORWARD ONLY(JUST SET the laser position nia)
             1. set the position back to ' ' FIRST.Otherwise you will have ------(BUT ONLY SET WHEN PREVIOUS POSITION IS LASER)
            if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
            {
                map[*laserRow][*laserColumn] = ' ';
            }*/
            
            /*update the laserRow FIRST(laserColumn remains unchanged).CHANGE THE COORDINATE FIRST N SEE WHAT IS DOES THIS POSITION HAS...*/
            *laserRow = *laserRow + 1;
        
            /*2. set the new position for the laser in the map*/
            map[*laserRow][*laserColumn] = *laser; 
          
        }
        /*If there is WALL in front when moving*/
        else if(map[*laserRow + 1][*laserColumn] == '*')
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*set the position back to ' ' FIRST.Otherwise you will have '|' on map when u ask for USER INPUT(after u exit the shootLaser()).BUT IF it is tank,then do nothing
            if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
            {
                map[*laserRow][*laserColumn] = ' ';
            }*/
        }
        /*If there is MIRROR in front when moving*/
        else if(map[*laserRow + 1][*laserColumn] == '\\' || map[*laserRow + 1][*laserColumn] == '/')
        {
            /*1. DON'T FORGET to set the previous laser as ' ' BEFORE u update the NEW laser position. Otherwise,you will have MULTIPLE laser appeared on the map.(BUT make this only if it is NOT THE TANK)
            if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
            {
                map[*laserRow][*laserColumn] = ' ';
            }*/
            
            /*THEN CHANGE THE DIRECTION/udpate the laser details*/
            newLaserPosition(laser,laserRow,laserColumn,&(map[*laserRow + 1][*laserColumn]),moveLeft,moveRight,moveUp,moveDown);
                            
            /*3. set the NEW laser position AFTER updating the laser details n movement(boolean) */
            /*!!!AFTER REFLECTING THE LASER,WE NEED TO CHECK IF IT IS THE TANK/MIRROR/WALL/SPACE AGAIN  SO...*/
            if(map[*laserRow][*laserColumn] != ' ')
            {
                setLaser(map,laser,laserRow,laserColumn,stopLaser,moveLeft,moveRight,moveUp,moveDown,stopProgram);
            }
            else/*If it is a space again,then we just set it*/
            {
                map[*laserRow][*laserColumn] = *laser; 
            }
        }
        /*If there is TANK in front when moving*/
        else
        {
            /*NEED TO STOP IT NOW*/
            *stopLaser = TRUE;
            
            /*DON'T FORGET TO SET THE LASER TO ' '.Otherwise, will have laser n X appear in the same map.(BUT only set to ' ' if it is  NOT THE TANK).We set it to ' ' when
             v <- player tank
             < <- enemy tank
            if(map[*laserRow][*laserColumn] == '-' || map[*laserRow][*laserColumn] == '|')
            {
                map[*laserRow][*laserColumn] = ' ';
            }*/
            
            /*UPDATE THE laser coordinate,SET THE LASER COORDINATE TO THE TANK'S COORDINATE
             WHY WE NEED TO DO THIS? As after we return to the shootLaser(),we need to DECIDE WHICH TANK GET HITTED.So, this laser coordinate HAVE TO BE UPDATED/MOVE ONE STEP FORWARD(!!BUT I DOES NOT SET THIS INTO THE MAP YA(How can I replace the tank position?))*/
            *laserRow = *laserRow + 1;
            
            /*END THE GAME*/
            *stopProgram = TRUE;
        }
    }
}

/*change (laser) to CORRECT position AFTER IT HITS THE MIRROR*/
void newLaserPosition(char* laser,int* laserRow,int* laserColumn,char* mirrorType,int* moveLeft,int* moveRight,int* moveUp,int* moveDown)
{
   
    /*UPDATE THE laser details and the movement(boolean)*/
    if(*mirrorType == '/')
    {
        if(*moveLeft == TRUE)/*means the laser COME FROM RIGHT side and hit the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '|';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserColumn = *laserColumn - 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveLeft = FALSE;
            *moveDown = TRUE;
        }
        else if(*moveRight == TRUE)/*means the laser COME FROM the LEFT side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '|';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserColumn = *laserColumn + 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveRight = FALSE;
            *moveUp = TRUE;
        }
        else if(*moveUp == TRUE)/*means the laser COME FROM the DOWN side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '-';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserRow = *laserRow - 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveUp = FALSE;
            *moveRight = TRUE;
        }
        else/* *moveDown == TRUE,means the laser COME FROM the UP side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '-';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserRow = *laserRow + 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveDown = FALSE;
            *moveLeft = TRUE;
        }
    }
    else/* *mirrorType == '\'*/
    {
        if(*moveLeft == TRUE)/*means the laser COME FROM RIGHT side and hit the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '|';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserColumn = *laserColumn - 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveLeft = FALSE;
            *moveUp = TRUE;
        }
        else if(*moveRight == TRUE)/*means the laser COME FROM the LEFT side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '|';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserColumn = *laserColumn + 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveRight = FALSE;
            *moveDown = TRUE;
        }
        else if(*moveUp == TRUE)/*means the laser COME FROM the DOWN side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '-';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserRow = *laserRow - 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveUp = FALSE;
            *moveLeft = TRUE;
        }
        else/* *moveDown == TRUE,means the laser COME FROM the UP side of the mirror*/
        {
            /*1. change the direction of the laser*/
            *laser = '-';
            /*2. change the coordinate of the laser(Move it to the mirror coordinate,BUT NOT SET IT INTO THE MAP!WE NEED TO CHECK AGAIN WHETHER WE GOT MIRROR/TANK/WALL IN FRONT OF THE THIS NEW LASER OR NOT.IF GOT, WE WILL NEED TO DECIDE THE NEW LASER POSITION AGAIN)*/
            *laserRow = *laserRow + 1;
            /*3.set the new direction now(update the new direction to move)*/
            *moveDown = FALSE;
            *moveRight = TRUE;
        }
    }

}
/************************************************************************
 * END: all method used for player tank shooting laser(option: f)
 **********************************************************************/

/************************************************************************
 * START: all method created for writing the linkedlist into the file
 **********************************************************************/
/*assignment from incompatible pointer type means the pointer type are different. BUT idk why i can Mirror** = Mirror** BUT cannot Map* = Map*(this is pointer to struct as I malloc it so I return Map*).HEADACHE....*/
/*Map* */
/*to copy the current map(for storing in the linkedlist)*/
char** copyMap(char** map,int* rowSize,int* columnSize)
{
    int i;
    char** copyMap = NULL;
    i = 0;
    
    copyMap = (char**)malloc(*rowSize * sizeof(char*));	
    for(i = 0 ; i < *rowSize ; i++)
    {
        copyMap[i] = (char*)malloc((*columnSize + 1) * sizeof(char));
    }
    
    /*Map* copyMap = (Map*)malloc(sizeof(Map));
    copyMap->map = map;*/
    
    /*copy the content of the map into copyMap*/
    for(i = 0 ; i < *rowSize ; i++)
    {
        strcpy(copyMap[i],map[i]); 
        copyMap[i][*columnSize] = '\0';
    }
       
return copyMap;
}

/*THIS IS TO free the map which allocated for storing inside the node
 it allocate at WHERE? the map in copyMap()*/
/*"Helper" function for freeLinkedList: to free the data in each list node(which are the char** copyMap)*/
void freeData(void* data,int* rowSize)
{
    int i;
    /*Using char** mapToFree as we cannot (char*)data[i]. SAME IDEA as I saw in the freeLinkedList(). Just make a pointer to the node and we free that node using the pointer(free(pointer))*/
    char** mapToFree = NULL;
    mapToFree = (char**)data;
    for(i = 0 ; i < *rowSize ; i++)
    {
        free((char*)mapToFree[i]);
    }
    free((char**)data);
}

/*FIRST TRIAL: FAIL as it shows an error: 
 error: pointer of type ‘void *’ used in arithmetic [-Werror=pointer-arith]
         free((char*)data[i]);
laserTank.c:320:25: error: dereferencing ‘void *’ pointer [-Werror]
laserTank.c:320:14: error: invalid use of void expression
         free((char*)data[i]);
void freeData(void* data,int* rowSize)
{
    int i;
    char** mapToFree = NULL;
    mapToFree = (char**)data;
    for(i = 0 ; i < *rowSize ; i++)
    {
        free((char*)data[i]);
    }
    free((char**)data);
}*/

/************************************************************************
 * END: all method created for writing the linkedlist into the file
 **********************************************************************/

