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

#define ARRAY_SIZE 2500

// DATA STRUCTURES
typedef struct node {
	void **array; // array of items held at location

	struct node *next; // pointer to next node
	struct node *prev; // pointer to previous node

	int first; // index of first slot in array
	int last; // index of last slot in array
	int count; // number of items in array
	int size; //how many indices there are in this node
	int full; //how many indices are full in this node
} NODE;

typedef struct list {
	int nodeCount; // count of nodes
	int itemCount; // count of items

	NODE *head; // pointer to first node in list
	NODE *tail; // pointer to last node in list
} LIST;

// MY ADDITIONAL FUNCTIONS

/* 
search: goes through list starting from head & ending at tail, returning the node at which the item at position index is located
runtime: O(n)
*/
NODE *search(LIST *lp, int index, int *loc) {
	NODE *node = lp->head; // start at first node
	int i; // for loop counter

	for(i = 0; i < lp->itemCount; i += node->count, node = node->next) { // loop through nodes
		if(index < node->count) {
			*loc = index; // update loc to be index
			return node;
		}		
		else
			index -= node->count;
	}
return NULL;
}

/*
createNode: sets up a new node & returns it
runtime: O(1)
*/
// NODE *createNode(LIST *lp) {
// 	NODE *newNode = malloc(sizeof(NODE));
// 	assert(newNode != NULL);

// 	newNode->count = 0; // set current count of items in array to zero
	
// 	//newNode->size = (lp->itemCount != 0) ? (lp->itemCount * lp->itemCount) : ARRAY_SIZE;
// 	newNode->size = pow(1, lp->nodeCount); //set size of array (dynamically increase size of arrays as you add new nodes)
// 	newNode->first = 0; // index of first element (empty right now)

// 	newNode->array = malloc(sizeof(void)* newNode->size); // allocate memory for the array
// 	assert(newNode->array != NULL); // make sure the array exists

// 	newNode->prev = NULL;
// 	newNode->next = NULL;

// 	lp->nodeCount++; // increase count of nodes in list
	
// 	return newNode; // return new node
// }

// FUNCTIONS FROM PROJECT INSTRUCTIONS

/*
createList: return a pointer to a new list
runtime: O(1)
*/
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST)); // allocate memory for list lp
	assert(lp != NULL); // make sure lp exists

	lp->nodeCount = 0; // initialize number of nodes in list (empty = 0)
	lp->itemCount = 0; // initialize number of items in list (empty = 0)

	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);

	//initialize next and prev pointers
	lp->head->next = NULL;
	lp->head->prev = NULL;
	
	//intialize the indices of the front and the back of the queue as 0
	lp->head->first = 0;
	lp->head->last = 0;

	//intitialize the number of full slots as 0 out of a total of 64 for the first node
	lp->head->full = 0;
	lp->head->size = 64;

	//allocate a void* array with as many indices as their are "slots"
	lp->head->array = malloc(sizeof(void*) * 64);

	lp->tail = lp->head;

	return lp; // return pointer to a new list
}

/*
destroyList: deallocate memory associated with the list pointed to by lp
runtime: O(n)
*/
void destroyList(LIST *lp) {
	assert(lp != NULL); // make sure lp exists
	NODE *pCur = lp->head; // first node in list
	NODE *pDel;

	while(pCur != NULL) { // go through list
		pDel = pCur->next;
		free(pCur->array);
		free(pCur);
		pCur = pDel;
	}

	free(lp); // free whole list
}

/*
numItems: return the number of items in the list pointed to by lp
runtime: O(1)
*/
int numItems(LIST *lp) {
	assert(lp != NULL); // make sure lp exists
	return lp->itemCount; // return number of items in the list
}

/*
addFirst: add item as the first item in the list pointed to by lp
runtime: O(1)
*/
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL);

	//if the head node is full then allocate a new node to accomodate new data
	if((lp->head->full) >= (lp->head->size)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = lp->head;
		newNode->prev = NULL;
		lp->head->prev = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->head->size);
	
		//set the newNode as the head of the linked list
		lp->head = lp->head->prev;

		//update slots
		lp->head->size = newSlots;

		//allocate an array of void* twice the size of the array in the node to the right
		lp->head->array = malloc(sizeof(void*) * (newSlots));
		assert(newNode-> array != NULL);

		//increment the counter of the number of nodes
		lp->nodeCount += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->head->full) < (lp->head->size));

	//decrease the index of the "first index"
	lp->head->first--;
	if(lp->head->first < 0){
		lp->head->first = lp->head->size - 1;
	}

	//put the item in the front of the queue in the current node
	//first represents the first index of the queue (not the array)
	//new first index is to the left of the current first index
	//lp->head->first = ((lp->head->first) - 1) % lp->head->slots;
	lp->head->array[lp->head->first] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->itemCount += 1;

	return;
}

/*
addLast: add item as the last item in the list pointed to by lp
runtime: O(1)
*/
void addLast(LIST *lp, void *item) {
	assert(lp != NULL);
	
	int index;

	//if the tail node is full then allocate a new node to accomodate new data
	if((lp->tail->full) >= (lp->tail->size)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = NULL;
		newNode->prev = lp->tail;
		lp->tail->next = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->tail->size);

		//set the newNode as the tail of the linked list
		lp->tail = lp->tail->next;

		//update slots
		lp->tail->size = newSlots;
	
		//allocate an array of void* twice the size of the array in the node to the left
		newNode->array = malloc(sizeof(void*) * (newSlots));
		assert(newNode->array != NULL);

		//increment the counter of the number of nodes
		lp->nodeCount += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->tail->full) < (lp->tail->size));

	//find the index where the last element should go
	index = lp->tail->first + lp->tail->full;
	if(index >= lp->tail->size){
		index = index - lp->tail->size;
	}

	//insert the item
	lp->tail->array[index] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->itemCount += 1;

	//lp->tail->data[((lp->tail->full) + (lp->tail->first)) % lp->tail->slots] = item;
	//update the "last" index number	
	lp->tail->last = ((lp->head->first) + (lp->head->full)) % lp->head->size;

	return;
}

/*
removeFirst: remove and return the first item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeFirst(LIST *lp) {
	assert(lp != NULL);

	//check that there is an item to remove and return
	assert(lp->itemCount > 0);

	//remove the head node if it is empty	
	if(lp->head->full == 0){

		NODE *delNode = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(delNode->array);
		free(delNode);
	}

	//store the first value in a temporary variable
	void* temp = lp->head->array[lp->head->first];
	
	//free(lp->head->data[lp->head->first]);
	
	//update the "first" index
	if(++lp->head->first >= lp->head->size)
		lp->head->first = 0;

	//lp->head->first = ((lp->head->first) + 1) % lp->head->slots;
	lp->itemCount -= 1;
	lp->head->full -= 1;

	return temp;
}

/*
removeLast: remove and return the last item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeLast(LIST *lp) {
	assert(lp != NULL);
	
	//check that there is an item to remove and return
	assert(lp->itemCount > 0);

	//remove the tail node if it is no longer needed
	if(lp->tail->full == 0){
		
		NODE *delNode = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(delNode->array);
		free(delNode);
	}

	//create a temporary value to store the last element
	void* temp = lp->tail->array[(lp->tail->full + lp->tail->first -1) % lp->tail->size];

	//free(lp->tail->data[lp->tail->last]);

	if(--lp->tail->last < 0){
		lp->tail->last = lp->tail->size - 1;
	}

	lp->itemCount -= 1;
	lp->tail->full -= 1;

	return temp;
}

/*
getItem: return the item at position index in the list pointed to by lp ; the index must be within range
runtime: O(n)
*/
void *getItem(LIST *lp, int index) {
	assert(lp != NULL);
	assert(index >= 0);
	assert(lp->itemCount > 0);
	
	//make sure the index is less than the total number of elements
	assert(index < lp->itemCount);

	//traverse linked list looking for the node that has the
	//array with the index that we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - pCur->full;
		pCur = pCur->next;
	}
	
	return pCur->array[(pCur->first + index) % pCur->size];
}

/*
setItem:
runtime:
*/
void setItem(LIST *lp, int index, void *item) {
	assert(lp != NULL);	

	//make sure the index is less than the total number of elements
	assert(index >= 0 && index < lp->itemCount);

	//like above, traverse the linked list looking for the node that has
	//the array with the index we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - (pCur->full);
		pCur = pCur->next;
	}

	//update the item at the given index
	pCur->array[(pCur->first + index) % pCur->size] = item;
}
