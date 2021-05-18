/* COEN 12 Term Project - File: list.c
 * Author: Madeleine Waldie
 * Due Date: 6/4/21
 */ 
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct node
{
	void **data;
	struct node *next;
	struct node *prev;
	int first;
	int count;
	int size;
}NODE;

struct list
{
	int nCount; 
	int iCount;
	NODE *head;
	NODE *tail;
};

static NODE *addNode(LIST *lp);
static NODE *search(LIST *lp, int index, int *loc);

// Allocates memory to the list structure, the head pointer, and the tail pointer
// O(1)
LIST *createList(void)
{
	LIST *lp;
	lp=malloc(sizeof(LIST));
	assert(lp!=NULL);
	lp->nCount=0;
	lp->iCount=0;
	lp->head=malloc(sizeof(NODE));
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
		free(p->data);
		p=p->next;	
	}
	free(lp);
}

// Returns the number of items in the list pointed to by lp
// O(1)
int numItems(LIST *lp)
{
	assert(lp!=NULL);
	return lp->iCount;
}

// If the current head node is full, then a new node is allocated and set as the new head node; In any case, the item is added to the front of the list, and the count is updated
// O(1)
void addFirst(LIST *lp, void *item)
{
	assert(lp!=NULL && item!=NULL);
	if(lp->nCount==0)
	{
		NODE *first=addNode(lp);
		lp->head=first;
		lp->tail=first;	
	}
	else if(lp->head->count==lp->head->size)
	{
		NODE *new=addNode(lp);	
		new->next=lp->head;
		lp->head->prev=new;
		lp->head=new;
	}
	lp->head->data[(lp->head->first+lp->head->count)%lp->head->size]=item;	
	lp->head->count++;
	lp->iCount++;	
}

// If the current tail node is full, then a new node is allocated and set as the new tail node; In any case, the item is added to the back of the list, and the count is updated
// O(1)
void addLast(LIST *lp, void *item)
{
	assert(lp!=NULL & item!=NULL);
	if(lp->nCount==0)
	{
		NODE *first=addNode(lp);
		lp->head=first;
		lp->tail=first;
	}
	else if(lp->tail->count==lp->head->size)
	{
		NODE *new=addNode(lp);
		new->prev=lp->tail;
		lp->tail->next=new;
		lp->tail=new;
	}
	lp->tail->data[(lp->tail->first+lp->tail->count)%lp->tail->size]=item;	
	lp->tail->count++;
	lp->iCount++;
}

// If the current head node is empty, then the node after it is set as the new head node, and the previous head node is freed; In any case, the item at the front of the list is removed, and the count is updated
// O(1)
void *removeFirst(LIST *lp)
{
	assert(lp!=NULL && lp->iCount>0);
	void *eDel;
	if(lp->head->count==0)
	{
		NODE *pDel=lp->head;
		lp->head=lp->head->next;
		lp->head->prev=NULL;
		free(pDel);
		lp->nCount--;
	}
	eDel=lp->head->data[lp->head->first];	
	lp->head->first=(lp->head->first+1)%lp->head->size;
	lp->head->count--;
	lp->iCount--;
	return eDel;
}

// If the current tail node is empty, then the node before it is set as the new tail node, and the previous tail node is freed; In any case, the item at the back of the list is removed, and the count is updated
// O(1)
void *removeLast(LIST *lp)
{
	assert(lp!=NULL && lp->iCount>0);
	void *eDel;
	if(lp->tail->count==0)
	{
		NODE *pDel=lp->tail;
		lp->tail=lp->tail->prev;
		lp->tail->next=NULL;
		free(pDel);
		lp->nCount--;
	}
	eDel=lp->tail->data[lp->tail->first];
	lp->tail->first=(lp->tail->first+1)%lp->tail->size;
	lp->tail->count--;
	lp->iCount--;
	return eDel;
}

// Ensures that the index is witin the bounds of the list; It receives the proper location of the index with regards to which node it is in and which index it is in within the node's array; Then, it returns the item at that index
// O(n)
void *getItem(LIST *lp, int index)
{
	assert(lp!=NULL && index>=0 && index<lp->iCount);
	int loc=0;
	NODE *p=search(lp, index, &loc);
	return p->data[(p->first+loc)%p->size];
}

// Ensures that the index is within the bounds of the list; It receives the proper location of the index with regards to which node it is in and which index it is in within the node's array; Then, it changes the value within the array to item
// O(n)
void setItem(LIST *lp, int index, void *item)
{
	assert(lp!=NULL && index>=0 && index<lp->iCount);
	int loc=0;
	NODE *p=search(lp, index, &loc);
	p->data[(p->first+loc)%p->size]=item;
}

// Allocates memory to a new node and its data and then it returns it
// O(1)
static NODE *addNode(LIST *lp)
{
	NODE *new=malloc(sizeof(NODE));
	assert(new!=NULL);
	new->count=0;
	new->size=pow(1,lp->nCount);
	new->first=0;
	new->data=malloc(sizeof(void)*new->size);
	assert(new->data!=NULL);
	new->prev=NULL;
	new->next=NULL;
	lp->nCount++;
	return new;
}
// The new->size is supposed to increase by a power of 2, however, I could not figure out how to correct the segmentation faults, so I figured it would be better to turn in something that works rather than something that does not

// Iterates through the list starting from the head pointer and ending at the tail pointer; If the index can be found within the current node, then the proper index within the node is indicated to the interface and the node itself is returned; Else, the pointer p moves to the next node
// O(n)
static NODE *search(LIST *lp, int index, int *loc)
{
	NODE *p=lp->head;
	int i;
	for(i=0;i<lp->iCount;i+=p->count,p=p->next)
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

// # include <stdio.h>
// # include <stdlib.h>
// # include <string.h>
// # include <assert.h>
// # include <stdbool.h>
// # include <math.h>
// # include "list.h"

// // data structures
// typedef struct node {
// 	void **array; // array of items held at location

// 	struct node *next; // pointer to next node
// 	struct node *prev; // pointer to previous node

// 	int first; // index of first slot in array
// 	int last; // index of last slot in array
// 	int size; //how many indices there are in this node
// 	int count; //how many indices are full in this node
// } NODE;

// typedef struct list {
// 	int nodeCount; // count of nodes
// 	int itemCount; // count of items

// 	NODE *head; // pointer to first node in list
// 	NODE *tail; // pointer to last node in list
// } LIST;

// /*
// createList: return a pointer to a new list
// runtime: O(1)
// */
// LIST *createList(void) {
// 	LIST *lp = malloc(sizeof(LIST)); // allocate memory for list lp
// 	assert(lp != NULL); // make sure lp exists

// 	lp->nodeCount = 0; // initialize number of nodes in list (empty = 0)
// 	lp->itemCount = 0; // initialize number of items in list (empty = 0)

// 	lp->head = malloc(sizeof(NODE));
// 	assert(lp->head != NULL);

// 	lp->head->next = NULL; //initialize next to be NULL
// 	lp->head->prev = NULL; //initialize next to be NULL
	
	
// 	lp->head->first = 0; // intialize the indices of the front of the queue as 0
// 	lp->head->last = 0; // intialize the indices of the back of the queue as 0

// 	lp->head->count = 0; // number of full slots is zero at start
// 	lp->head->size = 64; // make 64 slots available for the first node

// 	//allocate a void* array with as many indices as their are "slots"
// 	lp->head->array = malloc(sizeof(void*) * 64);

// 	lp->tail = lp->head; // set tail to head, as list is empty right now

// 	return lp; // return pointer to a new list
// }

// /*
// destroyList: deallocate memory associated with the list pointed to by lp
// runtime: O(n)
// */
// void destroyList(LIST *lp) {
// 	assert(lp != NULL); // make sure lp exists
// 	NODE *pCur = lp->head; // first node in list
// 	NODE *pDel; // to keep track of deleted node

// 	while(pCur != NULL) { // go through list
// 		pDel = pCur->next; // assign node to be deleted next
// 		free(pCur->array); // free current node's data
// 		free(pCur); // free current node
// 		pCur = pDel; // set current node to next node to be deleted
// 	}

// 	free(lp); // free whole list
// }

// /*
// numItems: return the number of items in the list pointed to by lp
// runtime: O(1)
// */
// int numItems(LIST *lp) {
// 	assert(lp != NULL); // make sure lp exists
// 	return lp->itemCount; // return number of items in the list
// }

// /*
// addFirst: add item as the first item in the list pointed to by lp
// runtime: O(1)
// */
// void addFirst(LIST *lp, void *item) {
// 	assert(lp != NULL); // make sure lp exists

// 	if((lp->head->count) >= (lp->head->size)){ //if the head is full, allocate a new node for more data

// 		NODE *newNode = malloc(sizeof(NODE)); // new node

// 		//put the new node in the linked list by reassigning pointers
// 		newNode->next = lp->head;
// 		newNode->prev = NULL;
// 		lp->head->prev = newNode;

// 		newNode->count = 0; // node is currently empty
// 		int newSize = 2 * (lp->head->size); // make new amount of slots equal double current size
// 		lp->head = lp->head->prev; //set the newNode as head
// 		lp->head->size = newSize; //update size

// 		lp->head->array = malloc(sizeof(void*) * (newSize)); // allocate memory for an array of twice the size of the prev array
// 		assert(newNode-> array != NULL); // make sure array exists

// 		lp->nodeCount += 1; // increment number of nodes
// 	}

// 	assert((lp->head->count) < (lp->head->size)); //make sure there's room to add an item

// 	lp->head->first--; //decrease the index of first
// 	if(lp->head->first < 0){
// 		lp->head->first = lp->head->size - 1; //new first index is to the left of the current first index
// 	}
	
// 	lp->head->array[lp->head->first] = item; //put the item in the front of the queue in the current node
	
// 	lp->head->count += 1; // update count
// 	lp->itemCount += 1; // update item count

// 	return;
// }

// /*
// addLast: add item as the last item in the list pointed to by lp
// runtime: O(1)
// */
// void addLast(LIST *lp, void *item) {
// 	assert(lp != NULL); // make sure lp exists
// 	int index; // variable to track index

// 	if((lp->tail->count) >= (lp->tail->size)){ // if tail is full then create a new node
// 		NODE *newNode = malloc(sizeof(NODE)); // allocate memory for new node

// 		newNode->next = NULL; // next is null because it's at end of list
// 		newNode->prev = lp->tail; // previous is old tail
// 		lp->tail->next = newNode; // add new node to list

// 		newNode->count = 0; // empty array currently
// 		int newSlots = 2 * (lp->tail->size); // double size of array

// 		lp->tail = lp->tail->next; // make new node tail

// 		lp->tail->size = newSlots; // update size
	
// 		newNode->array = malloc(sizeof(void*) * (newSlots)); // allocate memory for an array of twice the size of the prev array
// 		assert(newNode->array != NULL); // make sure array exists

// 		lp->nodeCount += 1; // increment num nodes
// 	}
// 	assert((lp->tail->count) < (lp->tail->size)); // make sure item can be added

// 	//find the index where the last element should go
// 	index = lp->tail->first + lp->tail->count;
// 	if(index >= lp->tail->size){
// 		index = index - lp->tail->size;
// 	}

// 	lp->tail->array[index] = item; //insert item
// 	lp->head->count += 1; // increment count
// 	lp->itemCount += 1; // increment num items
// 	lp->tail->last = ((lp->head->first) + (lp->head->count)) % lp->head->size; // reassign last (use formula from lab handout)

// 	return;
// }

// /*
// removeFirst: remove and return the first item in the list pointed to by lp ; the list must not be empty
// runtime: O(1)
// */
// void *removeFirst(LIST *lp) {
// 	assert((lp != NULL) && (lp->itemCount > 0)); // make sure lp exists & there are items in it

// 	if(lp->head->count == 0){ // remove & deallocate memory for head if it is empty	
// 		NODE *delNode = lp->head;
// 		lp->head = lp->head->next;
// 		lp->head->prev = NULL;
// 		free(delNode->array);
// 		free(delNode);
// 	}

// 	void* temp = lp->head->array[lp->head->first]; //store the first value in a temporary variable

// 	if(++lp->head->first >= lp->head->size) { //update the "first" index
// 		lp->head->first = 0;
// 	}

// 	lp->itemCount -= 1; // decrement num items
// 	lp->head->count -= 1; // decrement full

// 	return temp; // return item removed
// }

// /*
// removeLast: remove and return the last item in the list pointed to by lp ; the list must not be empty
// runtime: O(1)
// */
// void *removeLast(LIST *lp) {
// 	assert((lp != NULL) && (lp->itemCount > 0)); // make sure lp exists & there are items in it

// 	if(lp->tail->count == 0){ // remove & deallocate memory for tail if it is empty
// 		NODE *delNode = lp->tail;
// 		lp->tail = lp->tail->prev;
// 		lp->tail->next = NULL;
// 		free(delNode->array);
// 		free(delNode);
// 	}

// 	void* temp = lp->tail->array[(lp->tail->count + lp->tail->first -1) % lp->tail->size]; //create a temporary value to store the last element

// 	if(--lp->tail->last < 0){
// 		lp->tail->last = lp->tail->size - 1;
// 	}

// 	lp->itemCount -= 1; // decrement num items
// 	lp->tail->count -= 1; // decrement full

// 	return temp; // return removed item
// }

// /*
// getItem: return the item at position index in the list pointed to by lp ; the index must be within range
// runtime: O(n)
// */
// void *getItem(LIST *lp, int index) {
// 	assert((lp != NULL) && (index >= 0) && (lp->itemCount > 0) && (index < lp->itemCount));

// 	NODE *pCur = lp->head; // current node
// 	while(index >= pCur->count){ // go through list until index could be in a node
// 		index = index - pCur->count;
// 		pCur = pCur->next;
// 	}
	
// 	return pCur->array[(pCur->first + index) % pCur->size]; // return item at index
// }

// /*
// setItem: update the item at position index in the list pointed by lp (the index must be within range)
// runtime: O(n)
// */
// void setItem(LIST *lp, int index, void *item) {
// 	assert((lp != NULL) && (index >= 0) && (lp->itemCount > 0) && (index < lp->itemCount));

// 	NODE *pCur = lp->head; // current node
// 	while(index >= pCur->count){ // go through list until index could be in a node
// 		index = index - (pCur->count);
// 		pCur = pCur->next;
// 	}

// 	pCur->array[(pCur->first + index) % pCur->size] = item; //update the item at the given index
// }
