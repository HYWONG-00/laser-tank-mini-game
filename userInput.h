#ifndef STRUCT_HGUARD
#define STRUCT_HGUARD
typedef struct
{
	int row;
	int column;
	char type;
	/*char *text; We can use this but NOT RECOMMEND. AS we need to malloc for it.N need to free(extra work) Use malloc if question does not give a fixed size*/
}Mirror;

#endif

#ifndef USER_INPUT
	#define USER_INPUT
		int checkRange(int low,int high,int* rowOrColumn);
        int checkMapCoordinate(int* size);
        int tankCoordinate(int* rowSize,int* columnSize,int* tankRow,int* tankColumn);
        int checkTankDirection(char* direction);
        char playerInput(char** map,int* rowSize);
#endif

#ifndef BOOLEAN
	#define BOOLEAN
        #define FALSE 0     /*FALSE == 0*/
        #define TRUE !FALSE /*TRUE == 1*/
#endif

/*
#ifndef BETWEEN
	#define BETWEEN(low,high,value) (value >= low && value <= high)?0:1
#endif*/


