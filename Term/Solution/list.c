/* COEN 12 Term Project - File: list.c
 * Author: Madeleine Waldie
 * Due Date: 6/4/21
 */ 

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include <math.h>
# include "list.h"

// DATA STRUCTURES
typedef struct node {
	void **array; // array of items held at location

	struct node *next; // pointer to next node
	struct node *prev; // pointer to previous node

	int first; // index of first slot in array
	int count; // number of items in array
	int size; // size of array
} NODE;

typedef struct list {
	int nodeCount; // count of nodes
	int itemCount; // count of items

	NODE *head; // pointer to first node in list
	NODE *tail; // pointer to last node in list
} LIST;

// MY ADDITIONAL FUNCTIONS
static NODE *createNode(LIST *lp);
static NODE *search(LIST *lp, int index, int *loc);

// Allocates memory to the list structure, the head pointer, and the tail pointer
// O(1)
LIST *createList(void)
{
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->nodeCount = 0;
	lp->itemCount = 0;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head!=NULL);
	lp->tail=malloc(sizeof(NODE));
	assert(lp->tail!=NULL);
	return lp;
}

// Iterates through all of the nodes in the list pointed to by lp starting from the head pointer and ending at the tail pointer, freeing each node's void **data; After all of the list is cleared, it frees the pointer to the list structure
// O(n)
void destroyList(LIST *lp)
{
	assert(lp!=NULL);
	NODE *p=lp->head;
	while(p!=NULL)
	{
		free(p->array);
		p=p->next;	
	}
	free(lp);
}

// Returns the number of items in the list pointed to by lp
// O(1)
int numItems(LIST *lp)
{
	assert(lp!=NULL);
	return lp->itemCount;
}

// If the current head node is full, then a new node is allocated and set as the new head node; In any case, the item is added to the front of the list, and the count is updated
// O(1)
void addFirst(LIST *lp, void *item)
{
	assert(lp!=NULL && item!=NULL);
	if(lp->nodeCount==0)
	{
		NODE *first=createNode(lp);
		lp->head=first;
		lp->tail=first;	
	}
	else if(lp->head->count==lp->head->size)
	{
		NODE *new=createNode(lp);	
		new->next=lp->head;
		lp->head->prev=new;
		lp->head=new;
	}
	lp->head->array[(lp->head->first+lp->head->count)%lp->head->size]=item;	
	lp->head->count++;
	lp->itemCount++;	
}

// If the current tail node is full, then a new node is allocated and set as the new tail node; In any case, the item is added to the back of the list, and the count is updated
// O(1)
void addLast(LIST *lp, void *item)
{
	assert(lp!=NULL & item!=NULL);
	if(lp->nodeCount==0)
	{
		NODE *first=createNode(lp);
		lp->head=first;
		lp->tail=first;
	}
	else if(lp->tail->count==lp->head->size)
	{
		NODE *new=createNode(lp);
		new->prev=lp->tail;
		lp->tail->next=new;
		lp->tail=new;
	}
	lp->tail->array[(lp->tail->first+lp->tail->count)%lp->tail->size]=item;	
	lp->tail->count++;
	lp->itemCount++;
}

// If the current head node is empty, then the node after it is set as the new head node, and the previous head node is freed; In any case, the item at the front of the list is removed, and the count is updated
// O(1)
void *removeFirst(LIST *lp)
{
	assert(lp!=NULL && lp->itemCount>0);
	void *eDel;
	if(lp->head->count==0)
	{
		NODE *pDel=lp->head;
		lp->head=lp->head->next;
		lp->head->prev=NULL;
		free(pDel);
		lp->nodeCount--;
	}
	eDel=lp->head->array[lp->head->first];	
	lp->head->first=(lp->head->first+1)%lp->head->size;
	lp->head->count--;
	lp->itemCount--;
	return eDel;
}

// If the current tail node is empty, then the node before it is set as the new tail node, and the previous tail node is freed; In any case, the item at the back of the list is removed, and the count is updated
// O(1)
void *removeLast(LIST *lp)
{
	assert(lp!=NULL && lp->itemCount>0);
	void *eDel;
	if(lp->tail->count==0)
	{
		NODE *pDel=lp->tail;
		lp->tail=lp->tail->prev;
		lp->tail->next=NULL;
		free(pDel);
		lp->nodeCount--;
	}
	eDel=lp->tail->array[lp->tail->first];
	lp->tail->first=(lp->tail->first+1)%lp->tail->size;
	lp->tail->count--;
	lp->itemCount--;
	return eDel;
}

// Ensures that the index is witin the bounds of the list; It receives the proper location of the index with regards to which node it is in and which index it is in within the node's array; Then, it returns the item at that index
// O(n)
void *getItem(LIST *lp, int index)
{
	assert(lp!=NULL && index>=0 && index<lp->itemCount);
	int loc=0;
	NODE *p=search(lp, index, &loc);
	return p->array[(p->first+loc)%p->size];
}

// Ensures that the index is within the bounds of the list; It receives the proper location of the index with regards to which node it is in and which index it is in within the node's array; Then, it changes the value within the array to item
// O(n)
void setItem(LIST *lp, int index, void *item)
{
	assert(lp!=NULL && index>=0 && index<lp->itemCount);
	int loc=0;
	NODE *p=search(lp, index, &loc);
	p->array[(p->first+loc)%p->size]=item;
}

// MY ADDITIONAL FUNCTIONS

/*
createNode: sets up a new node & returns it; useful because new nodes are created several times in code
runtime: O(1)
*/
static NODE *createNode(LIST *lp)
{
	NODE *new=malloc(sizeof(NODE));
	assert(new!=NULL);
	new->count=0;
	new->size=pow(1, lp->nodeCount);
	new->first=0;
	new->array=malloc(sizeof(void)*new->size);
	assert(new->array!=NULL);
	new->prev=NULL;
	new->next=NULL;
	lp->nodeCount++;
	return new;
}

/* 
search: goes through list starting from head & ending at tail, returning the node at which the item at position index is located
runtime: O(n)
*/
static NODE *search(LIST *lp, int index, int *loc)
{
	NODE *p=lp->head;
	int i;
	for(i=0;i<lp->itemCount;i+=p->count,p=p->next)
	{
		if(index<p->count)
		{
			*loc=index;
			return p;
		}		
		else
			index-=p->count;	
	}
}