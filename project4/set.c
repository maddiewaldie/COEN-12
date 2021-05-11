/* COEN 12 Lab #4 - File: set.c
 * Author: Madeleine Waldie
 * Due Date: 5/16/21
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "set.h"
# include "list.h"

# define EMPTY 0
# define FILLED 1
# define DELETED 2

typedef struct set{
    int count;
    int length;
    LIST  **lists;
    int (*compare)();
    unsigned (*hash)();
} SET;

/*
createSet: return a pointer to a new set using compare as its comparison function, which may be NULL
runtime: O(n)
*/
SET *createSet(int maxElts,int (*compare)(),unsigned (*hash)()) {
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL); // make sure sp exists

    sp->count = 0; // set initial count to zero (because there's no data yet)
    sp->length = maxElts / 20; // set length to max # of elements divided by 20 (constant value provided)
    sp->lists = malloc(sizeof(LIST*) * sp->length);
    sp->compare = compare; // compare function
    sp->hash = hash; // hash function

    int i;
    for(i = 0; i < sp->length; i++) {
        sp->lists[i] = createList(compare); // at each spot in the array, create an empty list which will populate with nodes with data that hash to that array location
    }

    assert(sp->lists != NULL); // make sure lists exists
    return sp;
}

/*
destroySet: deallocate memory associated with the set pointed to by sp
runtime: O(n)
*/
void destroySet(SET *sp) {
    int i;
    for(i = 0; i < sp->length; i++) { // loop through set
        destroyList(sp->lists[i]); // free list memory at each location in set (destroyList runtime: O(n))
    }
      
    free(sp); // free memory associated with set
}

/*
numElements: return the number of elts in the set pointed to by sp
runtime: O(1)
*/
int numElements(SET *sp) {
    assert(sp != NULL); // make sure sp exists
    return sp->count; // return count
}

/*
addElement: add element to sp
runtime: O(n)
*/
void addElement(SET *sp,void  *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure sp and elt exist

    int i = (sp->hash)(elt) % sp->length; // expected hash index

    if(findItem(sp->lists[i], elt) == NULL) { // make sure elt isn't already in the list (findItem runtime: O(n))
        addFirst(sp->lists[i], elt); // if not in list, add element to list (addFirst runtime: O(1))
        sp->count++; // increase count by one
    }
}

/*
removeElement: if item is present in the set pointed to by sp then remove it; the comparison function must not be NULL
runtime: O(n)
*/
void removeElement(SET *sp, void  *elt) {
    assert((sp != NULL) && (elt != NULL)); // make sure sp and elt exist

    int i = (sp->hash)(elt) % sp->length; // expected hash index

    if(findItem(sp->lists[i], elt) != NULL){ // make sure elt is in the list (findItem runtime: O(n))
        removeItem(sp->lists[i], elt); // remove element from list (removeItem runtime: O(n))
        sp->count--; // decrease count by one
    }
}

/*
findElement: if item is present in the set pointed to by sp then return the matching item, otherwise return NULL; the comparison function must not be NULL
runtime: O(n)
*/
void  *findElement(SET *sp, void *elt) {
    assert(sp != NULL); // make sure sp exists
    int i = (sp->hash)(elt) % sp->length; // expected hash index
    return findItem(sp->lists[i], elt); // return if item is present, if not return NULL (findItem runtime: O(n))
}

/*
getElements: allocate and return an array of elements in the set pointed to by sp
runtime: O(n^2)
*/
void  *getElements(SET *sp) {
    assert(sp != NULL); // make sure sp exists
    void  **copy = malloc(sizeof(void*) * sp->count);
    assert(copy != NULL); // make sure copy exists

    int i, indx = 0; // counter vars for indices
    for(i = 0; i < sp->length; i++) { // go through set
        if(sp->lists[i] != NULL){ // only do when list at index i isn't NULL
            void **items = getItems(sp->lists[i]); // items at list index i to copy (getItems runtime: O(n))
            for(int j = 0; j < numItems(sp->lists[i]); j++){ // go through items to add to copy
                copy[indx] = items[j]; // add items to copy
                indx++;
            }
        }
    }

    return copy;
}