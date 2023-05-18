#ifndef LL_HGUARD
#define LL_HGUARD
/*struct LinkedListNode so that my *next know what is the datatype for it*/
typedef struct LinkedListNode
{
	void *data;/*generic value*/
	struct LinkedListNode *next;
}LinkedListNode;

typedef struct
{
	LinkedListNode *head;
	int count;
}LinkedList;

typedef void (*listFunc)(void* data,int* rowSize);
typedef void (*processFunc)(FILE* outPtr,void* data,int* rowSize);/*FILE* outPtr,char** map,int* rowSize*/

LinkedList* createLinkedList(void);
void insertStart(LinkedList* list, void* entry);
void* removeStart(LinkedList* list);
void insertLast(LinkedList* list, void* entry);
void* removeLast(LinkedList* list);

void printLinkedList(LinkedList* list,char* filename,processFunc funcPtr,int* rowSize);
void freeLinkedList(LinkedList* list, listFunc funcPtr,int* rowSize);

#endif
