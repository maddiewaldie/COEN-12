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
	assert(lp->head != NULL);
	lp->tail = malloc(sizeof(NODE));
	assert(lp->tail != NULL);
	return lp;
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

	lp->head->first++; // increment first
	if(lp->head->first == lp->head->size) { // special case
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
	return node->array[(node->first + loc)% node->size]; // return the item at position index in the list pointed to by lp
}

/*
setItem: changes item at index in list to a new item
runtime: O(n)
*/
void setItem(LIST *lp, int index, void *item)
{
	assert(lp != NULL && index >= 0 && index < lp->itemCount); // make sure lp exists, there are items in the list, & the index is valid
	int loc = 0;
	NODE *node = search(lp, index, &loc); // will update loc & return a node (runtime of search: O(n))
	node->array[(node->first + loc) % node->size] = item; // set array at index to new item
}

// MY ADDITIONAL FUNCTIONS

/*
createNode: sets up a new node & returns it; useful because new nodes are created several times in code
runtime: O(1)
*/
static NODE *createNode(LIST *lp)
{
	NODE *newNode = malloc(sizeof(NODE)); // allocate memory for node
	assert(newNode != NULL); // make sure node exists

	newNode->count = 0; // initially empty, so count is 0
	newNode->size = pow(1, lp->nodeCount); // size
	newNode->first = 0; // first element is initially 0

	newNode->array = malloc(sizeof(void)* newNode->size); // allocate memory for array
	assert(newNode->array != NULL); // make sure array exists

	newNode->prev = NULL; // set next prev node to null for now
	newNode->next = NULL; // set next node to null for now

	lp->nodeCount++; // increase node count

	return newNode; // return new node created
}

/* 
search: goes through list starting from head & ending at tail, returning the node at which the item at position index is located
runtime: O(n)
*/
static NODE *search(LIST *lp, int index, int *loc)
{
	NODE *node = lp->head; // start at first node
	int i; // for loop iterator

	for(i = 0; i < lp->itemCount; i += node->count, node = node->next) { // loop through list from head to tail
		if(index < node->count) { // check if index could be found in a node's array
			*loc = index; // this is the index in the array
			return node; // return node with array of index
		}		
		else { // need to continue going through list
			index -= node->count; // decrement index by the number of items in array	
		}
	}
	return node;
}
