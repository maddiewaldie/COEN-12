/* COEN 12 Lab #4 - File: list.c
 * Author: Madeleine Waldie
 * Due Date: 5/16/21
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "list.h"

typedef struct node{
    struct node *prev; // pointer to previous node
	void* data; // data held at location
	struct node *next; // pointer to next node
} NODE;

typedef struct list{
	int count; // number of nodes in list (not counting dummy node)
	NODE *head; // pointer to first node in list (dummy node)
	int (*compare)(); // compare function pointer
} LIST;

/*
createList: return a pointer to a new list using compare as its comparison function, which may be NULL
runtime: O(1)
*/

LIST *createList(int (*compare)()) {
    LIST * lp = malloc(sizeof(LIST));
	assert(lp != NULL); // make sure lp exists
	lp->count = 0; // set initial count to zero (because there's no data yet)

	NODE * dummy = malloc(sizeof(NODE)); // dummy node (always the first node in the list, but does not itself hold data)
	assert(dummy != NULL); // make sure dummy node exists
	dummy->next = dummy; 
	dummy->prev = dummy;

	lp->head = dummy; // set first node in lp to dummy node

	lp->compare = compare; // compare function (could be NULL)

	return lp; // return pointer to new list
}

/*
destroyList: deallocate memory associated with the list pointed to by lp
runtime: O(n)
*/
void destroyList(LIST *lp) {
    assert(lp != NULL); // make sure lp exists
    NODE *pDel = lp->head->prev;
    NODE *pPrev;

    while(pDel != lp->head){
        pPrev = pDel->prev;
        free(pDel);
        pDel=pPrev;
    }
    free(pDel);
    free(lp);
}

/*
numItems: return the number of items in the list pointed to by lp
runtime: O(1)
*/
int numItems(LIST *lp) {
    assert(lp != NULL); // make sure lp exists
    return lp->count;
}

/*
addFirst: add item as the first item in the list pointed to by lp
runtime: O(1)
*/
void addFirst(LIST *lp, void *item) {
    assert(lp != NULL); // make sure lp exists

	NODE *node  =malloc(sizeof(NODE)); // create new node
	assert(node!=NULL); // make sure node exists

	node->data = item; // assign item to new node as data
	node->prev = lp->head; // previous node is the dummy node, as this will be first item in list
	node->next = lp->head->next; // next node is what dummy node was pointing to
	lp->head->next->prev = node; // make next node's prev pointer point to new node
	lp->head->next = node; // reset dummy node's next pointer to point to new node

	lp->count++; // increase count by one
}

/*
addLast: add item as the last item in the list pointed to by lp
runtime: O(1)
*/
void addLast(LIST *lp, void *item) {
    assert(lp != NULL); // make sure lp exists

	NODE *node  =malloc(sizeof(NODE)); // create new node
	assert(node!=NULL); // make sure node exists

	node->data = item; // assign item to new node as data
	node->prev = lp->head->prev; // make node's prev pointer point to second to last element
	node->next = lp->head; // node's next pointer will point to dummy node, as it is at the end
	lp->head->prev->next = node; // make second to last element's next pointer point to new node
	lp->head->prev = node; // reset dummy node's prev pointer to point to new node

	lp->count++; // increase count by one
}

/*
removeFirst: remove and return the first item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeFirst(LIST *lp) {
    assert((lp != NULL) && (lp->count > 0)); // make sure lp exists & that list isn't empty

	NODE *node = lp->head->next; // node will be the first item in the list
    void* nodeData = node->data; // get data of first item

	lp->head->next = node->next; // take away pointer to first node
	node->next->prev = lp->head; // take away pointer to first node
	lp->count--; // decrease count by one


	free(node); // deallocate memory for node
	return nodeData; // return node's data
}

/*
removeLast: remove and return the last item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *removeLast(LIST *lp) {
    assert((lp != NULL) && (lp->count > 0)); // make sure lp exists & that list isn't empty
    
    NODE *node = malloc(sizeof(NODE));
	node = lp->head->prev; // node will be the last item in the list
    void* nodeData = node->data; // get data of last item

	lp->head->prev = node->prev; // take away pointer to last node
	node->prev->next = lp->head; // take away pointer to last node
	lp->count--; // decrease count by one

	free(node); // deallocate memory for node
	return nodeData; // return node's data
}

/*
getFirst: return, but do not remove, the first item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *getFirst(LIST *lp) {
    assert((lp != NULL) && (lp->count > 0)); // make sure lp exists & that list isn't empty
	return lp->head->next->data;
}

/*
getLast: return, but do not remove, the last item in the list pointed to by lp ; the list must not be empty
runtime: O(1)
*/
void *getLast(LIST *lp) {
    assert((lp != NULL) && (lp->count > 0)); // make sure lp exists & that list isn't empty
	return lp->head->prev->data;
}

/*
removeItem: if item is present in the list pointed to by lp then remove it; the comparison function must not be NULL
runtime: O(n)
*/
void removeItem(LIST *lp, void *item) {
    assert((lp != NULL) && (lp->compare != NULL)); // make sure lp exists & compare function isn't NULL

	if(lp->count > 0) { // only when there are items in the list
		NODE *node = lp->head->next; // set node to first item in list

		while(node != lp->head){ // go through list
			if((lp->compare)(node->data, item) == 0) { // two nodes match 
				node->prev->next = node->next; // get rid of pointer to item
				node->next->prev = node->prev; // get rid of pointer to item
				free(node); // free memory at node
				return; // job is done, can stop searching
			}
			node = node->next; // move onto next node
		}
	}
}

/*
findItem: if item is present in the list pointed to by lp then return the matching item, otherwise return NULL; the com- parison function must not be NULL
runtime: O(n)
*/
void *findItem(LIST *lp, void *item) {
    assert((lp != NULL) && (lp->compare != NULL)); // make sure lp exists & compare function isn't NULL

	if(lp->count > 0) { // only when there are items in the list
		NODE *node = lp->head->next; // set node to first item in list

		while(node != lp->head){ // loop through list
			if((lp->compare)(node->data,item) == 0) { // two nodes match 
                return node->data; // found the item, so you can return its data
            }
			node = node->next; // move onto next node
		}
	}

	return NULL; // item wasn't found, so return NULL
}

/*
getItems: allocate and return an array of items in the list pointed to by lp
runtime: O(n)
*/
void *getItems(LIST *lp) {
    assert(lp != NULL); // make sure lp exists

    void **copy = malloc(sizeof(void*)*lp->count); // copy array

    NODE *node = lp->head->next; // first node with actual data
    
    int i;
    for(i = 0; i < lp->count; i++) { // go through list and copy data over into copy array
        copy[i] = node->data;
        node = node->next; // move onto next node
    }
    return copy;
}
