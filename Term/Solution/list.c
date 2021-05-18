/* COEN 12 Term Project - File: list.c
 * Author: Madeleine Waldie
 * Due Date: 6/4/21
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node{                                                                            //node structure
        void **data;                                                                            //data array
        int length;                                                                             //length of array
        int first;                                                                              //position of first element in array
        int count;                                                                              //number of elements in the array

        struct node *next;                                                                      //next and prev pointers
        struct node *prev;
}NODE;


typedef struct list{                                                                            //list structure - doubly linked list
        NODE* tail;                                                                             //tail pointer
        NODE* head;                                                                             //head pointer
        int count;                                                                              //number of nodes in list
}LIST;

NODE *createNode(int length);

LIST *createList(void){                                                                         //createList function: O(1)
        LIST *lp = malloc(sizeof(LIST));                                                        //allocate memory for list pointer
        lp->head = lp->tail = createNode(8);                                                    //set head to empty node of size 8
        lp->count = 0;

        return lp;                                                                              //return list pointer
}

void destroyList(LIST *lp){                                                                     //destroyList function: O(n)
        assert(lp != NULL);

        NODE* pDel;
        while(lp->head != NULL){                                                                //traverse list and free each node and its data
                pDel = lp->head;
                lp->head = lp->head->next;
                free(pDel->data);
                free(pDel);
        }

        free(lp);                                                                               //free list
}

int numItems(LIST *lp){                                                                         //numItems function: O(1)
        assert(lp != NULL);

        return lp->count;                                                                       //returns number of items in list
}

void addFirst(LIST *lp, void *item){                                                            //addFirst function: O(1)
        assert(lp != NULL && item != NULL);

        if(lp->head->count == lp->head->length){                                                //check if first node is full
                NODE* pFirst = createNode(lp->head->length * 2);                                //create node with double array length
                pFirst->next = lp->head;                                                        //fix pointers
                lp->head = pFirst;
                pFirst->next->prev = pFirst;
        }

        int index = ((lp->head->first + lp->head->length - 1) % lp->head->length);              //calculate first index of array
        lp->head->data[index] = item;                                                           //insert item into first slot
        lp->head->first = index;

        lp->head->count++;                                                                      //increment counts
        lp->count++;
}

void addLast(LIST *lp, void *item){                                                             //addLast function: O(1)
        assert(lp != NULL && item != NULL);

        if(lp->tail->count == lp->tail->length){                                                //check if last node is full
                NODE* pLast = createNode(lp->tail->length * 2);                                 //create node with double array length
                pLast->prev = lp->tail;                                                         //fix pointers
                lp->tail->next = pLast;
                lp->tail = pLast;
        }

        int index = ((lp->tail->first + lp->tail->count) % lp->tail->length);                   //calculate last index of array
        lp->tail->data[index] = item;                                                           //insert item into last slot

        lp->tail->count++;                                                                      //increment counts
        lp->count++;
}

void *removeFirst(LIST *lp){                                                                    //removeFirst function: O(1)
        assert(lp != NULL);

        if(lp->head->count == 0){                                                               //check if array is empty
                lp->head = lp->head->next;
                free(lp->head->prev);                                                           //free node
                lp->head->prev = NULL;                                                          //fix pointers
        }

        int index = lp->head->first;                                                            //calculate next index to old first
        int newindex = (index+1) % lp->head->length;
        lp->head->first = newindex;                                                             //set new first to the index

        void *copy = lp->head->data[index];                                                     //copy array data

        lp->head->count--;                                                                      //decrement counts                                                              
        lp->count--;

        return copy;                                                                            //return copy
}

void *removeLast(LIST *lp){                                                                     //removeLast function: O(1)
        assert(lp != NULL);

        if(lp->tail->count == 0){                                                               //check if array is empty
                NODE* pDel = lp->tail;
                lp->tail = lp->tail->prev;                                                      //fix pointers
                lp->tail->next = NULL;
                free(pDel->data);                                                               //free node and node data
                free(pDel);
        }

        int index = ((lp->tail->first + lp->tail->count) % lp->tail->length);                   //calculate index of last item

        void* copy = lp->tail->data[index];                                                     //copy array data

        lp->tail->count--;                                                                      //decrement counts
        lp->count--;

        return copy;                                                                            //return copy
}

void *getItem(LIST *lp, int index){                                                             //getItem function: O(logn)
        assert(lp != NULL);
        assert(index >= 0 && index < lp->count);                                                //assert that given index is valid

        NODE* pTrav = lp->head;                                                                 //initialize traversal node

        while(index >= pTrav->count){                                                           //decrement index while traversing list
                index -= pTrav->count;
                pTrav = pTrav->next;
        }

        void* copy = pTrav->data[(pTrav->first + index) % pTrav->length];                       //copy array data of traversal node

        return copy;                                                                            //return copy
}

void setItem(LIST *lp, int index, void *item){                                                  //setItem funciton: O(logn)
        assert(lp != NULL && item != NULL);
        assert(index >= 0 && index < lp->count);                                                //assert that given index is valid

        NODE* pTrav = lp->head;                                                                 //initialize traversal node

        while(index >= pTrav->count){                                                           //decrement index while traversing list
                index -= pTrav->count;
                pTrav = pTrav->next;
        }

        pTrav->data[(pTrav->first + index) % pTrav->length] = item;                             //insert item at index of traversal node
}

NODE *createNode(int length){                                                                   //createNode function: O(1)
        NODE *np = malloc(sizeof(NODE));                                                        //allocate memory for node

        assert(np != NULL);

        np->data = malloc(sizeof(void**) * length);                                             //allocate memory for array
        np->count = 0;
        np->first = 0;

        np->length = length;
        np->next = NULL;
        np->prev = NULL;

        return np;                                                                              //return node pointer
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
