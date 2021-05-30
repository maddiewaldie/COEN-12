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

/* 
search: goes through list starting from head & ending at tail, returning the node at which the item at position index is located
runtime: O(n)
*/
NODE *search(LIST *lp, int index, int *loc) {
	NODE *node = lp->head; // start at first node
	//int i= 0; // counter
	
	while(index > node->count) { // go through list until index could be found in a node's array
		index -= node->count; // decrement index by the number of items in array
		node = node->next; // move on to next node
		//i += node->count; // increment i by the number of items in array
	}
	*loc = index; // update loc to be index
	return node; // return node at which item at index is located
}

/*
createNode: sets up a new node & returns it; useful because new nodes are created several times in code
runtime: O(1)
*/
NODE *createNode(LIST *lp) {
	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);

	newNode->count = 0; // set current count of items in array to zero
	newNode->size = pow(1, (lp->nodeCount - 1)) * 10; //2 * (lp->itemCount); //set size of array (dynamically increase size of arrays as you add new nodes)
	newNode->first = 0; // index of first element (empty right now)
	
	newNode->array = malloc(sizeof(void)* newNode->size); // allocate memory for the array
	assert(newNode->array != NULL); // make sure the array exists
	
	newNode->prev = NULL;
	newNode->next = NULL;
	
	lp->nodeCount++; // increase count of nodes in list
	
	return newNode; // return new node
}

// FUNCTIONS FROM PROJECT INSTRUCTIONS

/*
createList: return a pointer to a new list
runtime: O(1)
*/
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST)); // allocate memory for list lp
	assert(lp != NULL); // make sure lp exists

	lp->nodeCount = 1; // initialize number of nodes in list (empty = 0)
	lp->itemCount = 0; // initialize number of items in list (empty = 0)

	lp->head = malloc(sizeof(NODE)); // pointer to first node in list (empty right now)
	assert(lp->head != NULL); // make sure head exists

	lp->head->first = 0; // intialize the indices of the front of the queue as 0

	lp->head->count = 0; // number of full slots is zero at start
	lp->head->size = 10; // make 10 slots available for the first node

	lp->head->array = malloc(sizeof(void*) * 10); // allocate a void* array with as many indices as their are "slots"

	lp->tail = lp->head; // set tail to head, as list is empty right now

	return lp; // return pointer to a new list
}

/*
destroyList: deallocate memory associated with the list pointed to by lp
runtime: O(n)
*/
void destroyList(LIST *lp) {
	assert(lp != NULL); // make sure lp exists
	NODE *pCur = lp->head; // first node in list
	NODE *pDel; // to keep track of deleted node

	if(pCur == NULL) {
		free(lp);
		return;
	}

	while(pCur->next != NULL) { // go through list
		pDel = pCur; // assign node to be deleted next
		pCur = pCur->next;
		free(pDel->array); // free current node's data
		free(pDel); // free current node
		//pCur = pDel; // set current node to next node to be deleted
	}
	free(pCur->array);
	free(pCur);
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
	assert((lp != NULL) && (item != NULL)); // make sure lp & item exist

	if(lp->nodeCount == 0) { // if there are no nodes, add the first node
		NODE *firstNode = createNode(lp); // first node in array (runtime of createNode: O(1))
		lp->head = firstNode; // make head point to first node
		lp->tail = firstNode; // because this will be only node in list, also set tail to point to firstNode	
	}
	else if(lp->head->count == lp->head->size) { // if the first node is full, then a new node is allocated and set as the new head
		NODE *newFirstNode = createNode(lp); // create new first node (runtime of createNode: O(1))
		newFirstNode->next = lp->head;
		lp->head->prev = newFirstNode;
		lp->head = newFirstNode; // make head point to new first node
	}

	//decrease the index of the first index
	if(lp->head->count != 0){
		lp->head->first = (lp->head->first + lp->head->size - 1) % lp->head->size;
	}

	int index = lp->head->first; //(lp->head->first + lp->head->count) % lp->head->size; // index is (f +n − 1)%m (from lab handout)
	lp->head->array[index] = item; // add item at intended index

	lp->head->count++; // increase count of number of items in array
	lp->itemCount++; // increase count of number of items in list
}

/*
addLast: add item as the last item in the list pointed to by lp
runtime: O(1)
*/
void addLast(LIST *lp, void *item) {
	assert((lp!=NULL) && (item!=NULL)); // make sure lp & item exist

	if(lp->nodeCount == 0) { // if there are no nodes, add the first node 
		NODE *firstNode = createNode(lp); // first node in array is same as last node (runtime of createNode: O(1))
		lp->head = firstNode; // make head point to node
		lp->tail = firstNode; // make tail point to node
	}
	else if(lp->tail->count == lp->tail->size) { // if the last node is full, then a new node is allocated and set as the new tail
		NODE *newTailNode = createNode(lp); // create new tail node (runtime of createNode: O(1))
		newTailNode->prev = lp->tail;
		lp->tail->next = newTailNode;
		lp->tail = newTailNode; // make tail point to new tail node
	}

	int index = (lp->tail->first + lp->tail->count) % lp->tail->size; // index is (f +n − 1)%m (from lab handout)
	lp->tail->array[index] = item; // add item at intended index

	lp->tail->count++; // increase count of number of items in array
	lp->itemCount++; // increase count of number of items in list
}

/*
removeFirst: remove and return the first item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeFirst(LIST *lp) {
	assert((lp != NULL) && (lp->itemCount > 0)); // make sure lp exists and isn't empty

	void *itemToDelete; // create variable for item to be deleted

	if(lp->head->count == 0) { // if the first node in list is empty, delete & deallocate memory of first node
		NODE *pDel = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(pDel->array); // deallocate memory for array at pDel
		free(pDel); // deallocate memory for pDel
		lp->nodeCount--; // decrease node count by 1
	}

	itemToDelete = lp->head->array[lp->head->first]; // delete first element
	//lp->head->first = (lp->head->first + 1) % lp->head->size; // reset first pointer to new first element

	lp->head->first++;
	if(lp->head->first == lp->head->size) {
		lp->head->first = 0;
	}	

	lp->head->count--; // decrement count of items in array by one
	lp->itemCount--; // decrement count of items in list by one

	return itemToDelete; // return first item in list
}

/*
removeLast: remove and return the last item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeLast(LIST *lp) {
	assert((lp != NULL) && (lp->itemCount > 0)); // make sure lp exists and isn't empty

	void *itemToDelete; // create variable for item to be deleted

	if(lp->tail->count == 0) { // if the last node in list is empty, delete & deallocate memory of last node
		NODE *pDel = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(pDel); // deallocate memory for pDel
		lp->nodeCount--; // decrease node count by 1
	}

	itemToDelete = lp->tail->array[(lp->tail->first + lp->tail->count - 1) % lp->tail->size]; // delete last element
	//lp->tail->first = (lp->tail->first + 1) % lp->tail->size; // reset first pointer to new first element

	lp->tail->count--; // decrement count of items in array by one
	lp->itemCount--; // decrement count of items in list by one
	
	return itemToDelete; // return last item in list
}

/*
getItem: return the item at position index in the list pointed to by lp ; the index must be within range
runtime: O(n)
*/
void *getItem(LIST *lp, int index) {
	assert((lp != NULL) && (index >= 0) && (index < lp->itemCount)); // make sure lp exists & that index is within range
	int loc = 0;
	NODE * node = search(lp, index, &loc); // will update loc & return a node (runtime of search: O(n))
	return node->array[(node->first + loc - 1)% node->size]; // return the item at position index in the list pointed to by lp
}


/*
setItem: changes item at index in list to a new item
runtime: O(n)
*/
void setItem(LIST *lp, int index, void *item)
{
	assert(lp != NULL && index >= 0 && index < lp->itemCount); // make sure lp exists, there are items in the list, & the index is valid
	int loc = 0;
	NODE *p = search(lp, index, &loc); // will update loc & return a node (runtime of search: O(n))
	p->array[(p->first + loc - 1) % p->size] = item; // set array at index to new item
}