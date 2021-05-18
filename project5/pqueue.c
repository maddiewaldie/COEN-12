/* COEN 12 Project 5 - File: pqueue.c
 * Author: Madeleine Waldie
 * Due Date: 5/30/21
 */ 

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "pqueue.h"

#define L(x) 2*x + 1 // left child
#define P(x) (x-1)/2 // parent
#define R(x) 2*x + 2 // right child

typedef struct pqueue {
    int count; // number of entries in array
    int length; // length of allocated array
    void **data; // allocated array of entries
    int (*compare)(); //comparison function
} PQ;

/*
createQueue: return a pointer to a new priority queue using compare as its comparison function
runtime: O(1)
*/
PQ *createQueue(int (*compare)()) {
    PQ *pq = malloc(sizeof(PQ)); // allocate memory for pq
	assert(pq!=NULL); // make sure pq exists
	
	pq->length = 10; // start length is 10
	pq->count = 0; // initial count is 0

    pq->compare = compare; // comparison function

	pq->data = malloc(sizeof(void*)*pq->length); // allocate memory for data
	assert(pq->data != NULL); // make sure data exists

	return (pq); // return pointer to new priority queue
}

/*
destroyQueue: deallocate memory associated with the priority queue pointed to by pq
runtime: O(n)
*/
void destroyQueue(PQ *pq) {
    assert(pq != NULL); // make sure pq exists

	int i; // for loop iterator

	for (i = 0; i < pq->count; i++) { // go through pq
        free(pq->data[i]); // deallocate memory for data at each i
    }
	free(pq->data); // deallocate memory for data as a whole
	free(pq); // deallocate memory for pq
}

/*
numEntries: return the number of entries in the priority queue pointed to by pq
runtime: O(1)
*/
int numEntries(PQ *pq) {
    assert(pq != NULL); // make sure pq exists
	return pq->count; // return number of entries
}

/*
addEntry: add entry to the priority queue pointed to by pq
runtime: O(logn)
*/
void addEntry(PQ *pq, void *entry) {
    assert((pq != NULL) && (entry != NULL)); // make sure pq & entry exist

	if (pq->count == pq->length) { // reallocate memory for array if it's completely filled
		pq->data = realloc(pq->data, sizeof(void*) *pq->length * 2);	// double its size
		pq->length = pq->length * 2;
	}

	pq->data[pq->count] = entry; // add the entry to the end of data
	int i = pq->count; // to keep track of where we are in the array in while loop

	while (pq->compare(pq->data[i], pq->data[P(i)]) < 0) {	// reheap up; continually compare with parent; if child is smaller than parent, then we swap
		void* temp = pq->data[P(i)];
		pq->data[P(i)] = pq->data[i];
		pq->data[i] = temp;
		i = P(i);
	}
	pq->count++; // increase count of elements in pq
}

/*
removeEntry: remove and return the smallest entry from the priority queue pointed to by pq
runtime: O(logn)
*/
void *removeEntry(PQ *pq) {
    void* root = pq->data[0]; // root is first element in array
	int i = 0, smallestIndex = 0; // i to track the i we are at when we swap; smallestIndex to see which child is smallest
	pq->data[i] = pq->data[pq->count-1]; // replace the first element with the last
	pq->count--; // decrement the number of entries in array

	while (L(i) < pq->count) { // loop through pq while we still have a left child
		smallestIndex = L(i); // set current smallest indx to i of left child

		if (R(i)<pq->count) { // right child; compare between the two children
			if (pq->compare(pq->data[L(i)], pq->data[R(i)]) < 0){
				smallestIndex = L(i); // left child is smaller
			} else {
				smallestIndex = R(i); // right child is smaller
			}
		}
		
		if (pq->compare(pq->data[smallestIndex], pq->data[i]) < 0) { //if it is smaller than the parent, then we swap
			void* temp = pq->data[smallestIndex];
			pq->data[smallestIndex] = pq->data[i];
			pq->data[i] = temp;
			i = smallestIndex;											
		}
		else { // otherwise go out of loop
			break;
		}
	}
	return root; // return root entry		
}