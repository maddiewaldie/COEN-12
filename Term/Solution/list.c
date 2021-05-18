/* COEN 12 Term Project - File: list.c
 * Author: Madeleine Waldie
 * Due Date: 6/4/21
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

//Node struct definition
typedef struct node {
	int slots; //how many indices there are in this node
	int full; //how many indices are full in this node
	struct node *next; //points to the next node
	struct node *prev; //points to the previous node
	int first; //index of the first element in the queue
	int last; //index of the last element in the queue
	void **data; //points to the array of generic data pointers
}NODE;

//List struct definition
typedef struct list {
	int totalItems; //total number of elements in all of the nodes
	int count; //number of nodes in the list
	NODE *head; //points to the head node
	NODE *tail; //points to the tail node
}LIST;

/*
As nodes are added the nodes should increase
in size so that less operations have to be done to find an element
I will start looking for a particular item from the front
*/

//function that creates a node
// O(1) time complexity
/*
static NODE* createNode(int slots){
	NODE *np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->next = NULL;
	np->prev = NULL;
	
	np->slots = slots;
	np->first = 0;
	np->last = 0;
	
	np->full = 0;
	np->data = malloc(sizeof(void*) * slots);
	assert(np->data != NULL);
	return np;
}
*/

//return a pointer to a new list
// O(1) time complexity
LIST *createList(void){

	LIST *lp;
	lp = malloc(sizeof(LIST));

	assert(lp != NULL);

	lp->totalItems = 0; //initialize the number of elements in the list to 0
	lp->count = 0; //initialize the number of nodes in the list to 0

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
	lp->head->slots = 64;

	//allocate a void* array with as many indices as their are "slots"
	lp->head->data = malloc(sizeof(void*) * 64);

	lp->tail = lp->head;
	return lp;
}

// deallocate memory associated with the list pointed by lp
// O(log n) time comlexity because the number of nodes grows with logarithmic complexity
//so we will only need to do proportionately less operations to free the whole list as the list gets larger
void destroyList(LIST *lp){

	assert(lp != NULL);
	NODE *pCur = lp->head;
	NODE *pDel;	

	//iterate through the linked list and deallocate nodes
	while(pCur != NULL){
	
		pDel = pCur->next;
		free(pCur->data);
		free(pCur);
		pCur = pDel;
		//pDel = pCur;
		//pCur = pCur->next;
		//free(pDel);
	}

	free(lp);
}

//return the number of items in the list pointed by lp
// O(1) time complexity
int numItems(LIST *lp){

	assert (lp != NULL);
	return lp->totalItems;	
}

//add item as the first item in the list pointed by lp
// O(1) time complexity
void addFirst(LIST *lp, void *item){

	assert(lp != NULL);

	//if the head node is full then allocate a new node to accomodate new data
	if((lp->head->full) >= (lp->head->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = lp->head;
		newNode->prev = NULL;
		lp->head->prev = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->head->slots);
	
		//set the newNode as the head of the linked list
		lp->head = lp->head->prev;

		//update slots
		lp->head->slots = newSlots;

		//allocate an array of void* twice the size of the array in the node to the right
		lp->head->data = malloc(sizeof(void*) * (newSlots));
		assert(newNode-> data != NULL);

		//increment the counter of the number of nodes
		lp->count += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->head->full) < (lp->head->slots));

	//decrease the index of the "first index"
	lp->head->first--;
	if(lp->head->first < 0){
		lp->head->first = lp->head->slots - 1;
	}

	//put the item in the front of the queue in the current node
	//first represents the first index of the queue (not the array)
	//new first index is to the left of the current first index
	//lp->head->first = ((lp->head->first) - 1) % lp->head->slots;
	lp->head->data[lp->head->first] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->totalItems += 1;

	return;
}

//add item as the last item in the list pointed by lp
// O(1) time complexity
void addLast(LIST *lp, void *item){

	assert(lp != NULL);
	
	int index;

	//if the tail node is full then allocate a new node to accomodate new data
	if((lp->tail->full) >= (lp->tail->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = NULL;
		newNode->prev = lp->tail;
		lp->tail->next = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->tail->slots);

		//set the newNode as the tail of the linked list
		lp->tail = lp->tail->next;

		//update slots
		lp->tail->slots = newSlots;
	
		//allocate an array of void* twice the size of the array in the node to the left
		newNode->data = malloc(sizeof(void*) * (newSlots));
		assert(newNode->data != NULL);

		//increment the counter of the number of nodes
		lp->count += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->tail->full) < (lp->tail->slots));

	//find the index where the last element should go
	index = lp->tail->first + lp->tail->full;
	if(index >= lp->tail->slots){
		index = index - lp->tail->slots;
	}

	//insert the item
	lp->tail->data[index] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->totalItems += 1;

	//lp->tail->data[((lp->tail->full) + (lp->tail->first)) % lp->tail->slots] = item;
	//update the "last" index number	
	lp->tail->last = ((lp->head->first) + (lp->head->full)) % lp->head->slots;

	return;
}

//remove and return the first item pointed to by lp (the list must not be empty)
// O(1) time complexity
void *removeFirst(LIST *lp){

	assert(lp != NULL);

	//check that there is an item to remove and return
	assert(lp->totalItems > 0);

	//remove the head node if it is empty	
	if(lp->head->full == 0){

		NODE *delNode = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(delNode->data);
		free(delNode);
	}

	//store the first value in a temporary variable
	void* temp = lp->head->data[lp->head->first];
	
	//free(lp->head->data[lp->head->first]);
	
	//update the "first" index
	if(++lp->head->first >= lp->head->slots)
		lp->head->first = 0;

	//lp->head->first = ((lp->head->first) + 1) % lp->head->slots;
	lp->totalItems -= 1;
	lp->head->full -= 1;

	return temp;
}

//remove and retur the last item in the list pointed by lp (the list must not be empty)
// O(1) time complexity
void *removeLast(LIST *lp){

	assert(lp != NULL);
	
	//check that there is an item to remove and return
	assert(lp->totalItems > 0);

	//remove the tail node if it is no longer needed
	if(lp->tail->full == 0){
		
		NODE *delNode = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(delNode->data);
		free(delNode);
	}

	//create a temporary value to store the last element
	void* temp = lp->tail->data[(lp->tail->full + lp->tail->first -1) % lp->tail->slots];

	//free(lp->tail->data[lp->tail->last]);

	if(--lp->tail->last < 0){
		lp->tail->last = lp->tail->slots - 1;
	}

	lp->totalItems -= 1;
	lp->tail->full -= 1;

	return temp;		
}

//return the item at position index in the list pointed to by lp (the index must be within range)
// worst case O(log n) runtime if the index we are looking for is at the end of the list we
// will have to iterate through the entire list but the nodes have varying size
void *getItem(LIST *lp, int index){

	assert(lp != NULL);
	assert(index >= 0);
	assert(lp->totalItems > 0);
	
	//make sure the index is less than the total number of elements
	assert(index < lp->totalItems);

	//traverse linked list looking for the node that has the
	//array with the index that we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - pCur->full;
		pCur = pCur->next;
	}
	
	return pCur->data[(pCur->first + index) % pCur->slots];
}

//update the item at position index in the list pointed by lp (the index must be within range)
// worst case O(log n) runtime because we may have to iterate through the whole list but
//as the list grows the number of nodes increases logarithmically
void setItem(LIST *lp, int index, void *item){

	assert(lp != NULL);	

	//make sure the index is less than the total number of elements
	assert(index >= 0 && index < lp->totalItems);

	//like above, traverse the linked list looking for the node that has
	//the array with the index we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - (pCur->full);
		pCur = pCur->next;
	}

	//update the item at the given index
	pCur->data[(pCur->first + index) % pCur->slots] = item;
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
