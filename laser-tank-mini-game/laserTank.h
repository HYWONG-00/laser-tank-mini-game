#ifndef STRUCT_HGUARD
#define STRUCT_HGUARD
typedef struct
{
	int row;
	int column;
	char type;
	/*char *text; We can use this but NOT RECOMMEND. AS we need to malloc for it.N need to free(extra work) Use malloc if question does not give a fixed size*/
}Mirror;
/*
typedef struct
{
	char** map;
}Map;*/
#endif

#ifndef LASER_TANK
#define LASER_TANK
	Mirror** convertFile(FILE* inPtr,int* rowSize,int* columnSize,int* playerRow,int* playerColumn,char* playerDirection,int* enemyRow,int* enemyColumn,char* enemyDirection,int* mirrorsNum,int* mirrorInMap,int* correctMirrorType);
	void firstTankDirection(char* tank);	
    void playerTankDirection(char letter,char* tank);
    int playerInFrontEnemy(char** map,char* enemyDirection,int* enemyRow,int* enemyColumn,int* rowSize,int* columnSize,int* playerRow,int* playerColumn);
    void shootPlayer(char** map,int* rowSize,int* columnSize,char* playerDirection,int* playerRow,int* playerColumn,char* enemyDirection,int* enemyRow,int* enemyColumn,LinkedList* list,char* filename);
    void shootLaser(char** map,int* rowSize,int* columnSize,char* playerDirection,int* stopProgram,int* playerRow,int* playerColumn,int* enemyRow,int* enemyColumn,LinkedList* list,char* filename);
    void setLaser(char** map,char* laser,int* laserRow,int* laserColumn,int* stopLaser,int* moveLeft,int* moveRight,int* moveUp,int* moveDown,int* stopProgram);
    void newLaserPosition(char* laser,int* laserRow,int* laserColumn,char* mirrorType,int* moveLeft,int* moveRight,int* moveUp,int* moveDown);
    char** copyMap(char** map,int* rowSize,int* columnSize);
    void freeData(void* data,int* rowSize);
#endif

    
