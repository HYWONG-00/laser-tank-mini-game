/*REFERENCES: 
 Obtained from Practical 06*/

#include <stdlib.h>/*for malloc*/
#include <stdio.h>
#include "linkedlist.h"

LinkedList* createLinkedList(void)
{
	LinkedList *LL = NULL;
	LL = (LinkedList*)malloc(sizeof(LinkedList));
	LL->head = NULL;
    LL->count = 0;
    return LL;/*return the address of LL*/
}

/*insert as the first node(always reset the head position)*/
void insertStart(LinkedList* list, void* entry)
{
        LinkedListNode *newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        /*see the pointer diagram in worksheet*/
        /*1*/newNode->data = entry;
        newNode->next = list->head;/*2. list's head haven't change,so we set the newndoe's next points to the first node then..*/
        /*3we reset the list's head*/list->head = newNode;
        /*4*/list->count++;
}

/*remove the first node(always reset the head position)*/
void* removeStart(LinkedList* list)
{
    void* data = NULL;
    LinkedListNode* firstNode = list->head;
    /*make sure the linkedlist is NOT EMPTY*/
    if(firstNode != NULL)/*list->count != 0 I think?*/
    {
        data = firstNode->data;
        /*reset the head first before u free the node*/
        list->head = firstNode->next;
        free(firstNode);
        /*Don't forget to keep track yr list's count*/
        list->count--;
    }
    return data;
}

void insertLast(LinkedList* list, void* entry)
{
    if(list->head == NULL)
    {
        LinkedListNode *newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        /*1*/newNode->data = entry;
        newNode->next = NULL;
        list->head = newNode;
        /*4*/list->count++;
    }
    else
    {
        LinkedListNode* curr;
        /*1. create a list node 1st*/
        LinkedListNode* newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
        newNode->data = entry;
        newNode->next = NULL;
        
        /*2. start find the last node in the list*/
        curr = list->head;
        while(curr->next != NULL)
        {
            curr = curr->next;
        }
        /*curr = last node in the list NOW*/
        /*3. points the original last node's next to the newNode*/
        curr->next = newNode;
        /*4. Don't forget to keep track how many list node we have*/
        list->count++;
    }
    /*printf("%d  %d  %d: %s\n",((JEntry*)entry)->day,((JEntry*)entry)->month,((JEntry*)entry)->year,((JEntry*)entry)->text); to see the data inserted at here*/
}

void* removeLast(LinkedList* list)
{
    void* data = NULL;
    LinkedListNode* curr;
    LinkedListNode* secondLastNode;
    curr = list->head;
    /*make sure the linkedlist is NOT EMPTY*/
    if(list->head == NULL)
    {
        printf("removeLast: List is EMPTY.Nothing to remove...");
    }
    else
    {
        /*1. start find the last node in the list*/
        while(curr->next != NULL)
        {
            secondLastNode = curr;
            curr = curr->next;
        }
        /*curr = last node in the list NOW*/
        /*secondLastNode = second last node now*/
        /*3. points the original second last node's next to the null*/
        secondLastNode->next = NULL;
        /*4. return the value with the last node's data*/
        data = curr->data;
        /*5. Don't forget to free the last node*/
        free(curr);
        /*6. Don't forget to keep track how many list node we have*/
        list->count--;
    }
return data;
}

/*NO NEED TO PASS char** map as we will GET curr->data(which is char** map stored in listnode)n pass this to the processMap function(to write the map into the file)*/
void printLinkedList(LinkedList* list,char* filename,processFunc funcPtr,int* rowSize)
{
    FILE* outPtr;
    LinkedListNode* curr;
    
    /*open the file to write*/
    outPtr = fopen(filename,"w");
    
    curr = list->head;/*1.curr pointer points to the first node*/
    
    while(curr != NULL)/*loop until last node*/
    {
        /*2. Write the current node's data(char** map)into the file*/
        (*funcPtr)(outPtr,curr->data,rowSize);/*outPtr,map,rowSize*/
        /*(*processFunc)(outPtr,curr->data,rowSize); u will get error: expected expression before ‘processFunc’
...it should be the VARIABLE, NOT DATATYPE!!!*/
        /*3. move to the next listnode*/
        curr = curr->next;
    }
    fclose(outPtr);
}

/*If your structure got malloc something(for class fields,maybe is string) need to free it as well*/
void freeLinkedList(LinkedList* list, listFunc funcPtr,int* rowSize)
{
    LinkedListNode* curr = list->head;/*1.curr pointer points to the first node*/
    LinkedListNode* nextCurr = NULL;
    
    while(curr != NULL)
    {   
        /*1.point to the next node FIRST. MUST DO THIS BEFORE YOU free the curr(current list node) as if u don't, HOW U MOVE TO THE NEXT NODE AFTER U FREE IT?u try curr->next will get NULL*/
        nextCurr = curr->next;
        
        /*Why we use function pointer? As every structure are different.Thus u need to make sure u dun hv malloc anything in yr structure b4 u free this. IF U HV CLASS FIELDS that have using heap memory, MUST free that class fields first!!! */
        (*funcPtr)(curr->data,rowSize);/*2.free the data pointer*/
        free(curr);/*3. free the CURRENT list node*/
        curr = nextCurr;/*4. MOVE to the next node*/
    }
    /*5.free the LinkedList*/
    free(list);
}




