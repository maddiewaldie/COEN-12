/* COEN 12 Lab #2 - File: table.c
 * Author: Madeleine Waldie
 * Due Date: 5/2/21
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"

// Flag constant values
# define EMPTY 0 // empty flag
# define FILLED 1 // filled flag
# define DELETED 2 // deleted flag

typedef struct set {
	int count; // num elements
	int length; // length of array
	char **elts; // array
	char *flags; // array of flags (E - empty, F - filled, D - deleted)
} SET;

/*
strhash: assigns a unique hash value to each elt by running through each character in the elt, multiplying its value by 31, and adding those values together
runtime: O(1)
*/ 
unsigned strhash(char *s) {

	unsigned hash = 0;

	while(*s != '\0')
		hash = 31 * hash + *s ++;

	return hash;
}

/*
search: uses linear probing to find if an element is in the array, has been deleted from the array, or is not in the array
runtime: worst: O(n); expected: O(1)
*/ 
int search(SET *sp, char *elt, bool *found) {
	assert((sp != NULL) && (elt != NULL)); // make sure both sp & elt exist

	int h = strhash(elt) % sp->length; // h is the hash value for the element
	int i, loc; // i is incrementer; loc is element location
	int deletedloc = -1; 

	for(i = 0; i < sp->length; i++) { // go through elts
		loc = (h + i) % sp->length; // increment hash value by i
		if(sp->flags[loc] == FILLED) { // array location has some element
			if(strcmp(sp->elts[loc], elt) == 0) { // both elts are equal
				*found = true; // found the element!
				return loc; // location of element
			}
		}
		else if(sp->flags[loc] == DELETED) { // array location has a deleted element
			if(deletedloc == -1) {
				deletedloc = loc; // keep track of the first deleted location
			}
		}
		else { // must not be in the array
			break;
		}
	}

	*found = false; // the element has not been found, so set found to false

	return (deletedloc == -1) ? loc : deletedloc; // if deletedloc equals -1, return loc, otherwise return deletedloc
}

/*
createSet: return a pointer to a new set with a maximum capacity of maxElts
runtime: O(n)
*/
SET *createSet(int maxElts) {
	SET*sp = malloc(sizeof(SET)); // declare and allocate memory for set
	assert(sp != NULL); // check to make sure memory was allocated

	sp->count = 0; // set count to number of elements currently in set (0 right now)
	sp->length = maxElts; // set length to length of array
	sp->elts = malloc(sizeof(char*)*maxElts); // allocate memory for array
	sp->flags = malloc(sizeof(char)*maxElts); // allocate memory for array
	
	int i;
	for(i = 0; i < maxElts; i++) {
		sp->flags[i] = EMPTY; // assign each empty elt an empty flag
	}

	assert(sp->elts != NULL); // check to make sure memory was allocated

	return sp; // returns a new set
}

/*
destroySet: deallocate memory associated with the set pointed to by sp
runtime: O(n)
*/
void destroySet(SET *sp) {
	int i;
	for(i = 0; i < sp->length; i++) { // go through array
		if(sp->flags[i] == FILLED) {
			free(sp->elts[i]); // deallocate memory only for filled slots in array
		}
	}
	free(sp->flags); //deallocate memory of the array of flags
	free(sp->elts); // deallocate memory of the array
	free(sp); //  deallocate memory of the set
}

/*
numElements: return the number of elements in the set pointed to by sp
runtime: O(1)
*/
int numElements(SET *sp) {
	assert(sp != NULL); // make sure sp exists
	return sp->count; // returns number of elts in sp
}

/*
addElement: add elt to the set pointed to by sp
runtime: worst: O(n); expected: O(1)
*/
void addElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL); // check that sp & elt exist

	bool found;
	int index = search(sp, elt, &found); // this is the index where the element should go

	if (!found) {
		sp->elts[index] = strdup(elt); // add element to the array
		sp->flags[index] = FILLED; // set flag to filled
		sp->count++; // increment number of items in array
	}
}

/*
removeElement: remove elt from the set pointed to by sp
runtime: worst: O(n); expected: O(1)
*/
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL); // make sure that both sp & elt exist

	bool found;
	int index = search(sp, elt, &found); // this is the index where the element should be

	if (found) {
		free(sp->elts[index]); // deallocate memory at index
		sp->flags[index] = DELETED; // set flag to deleted
		sp->count--; // decrement number of items in array
	}
}

/*
findElement: if elt is present in the set pointed to by sp then return the matching element, otherwise return NULL
runtime: worst: O(n); expected: O(1)
*/
char *findElement(SET *sp, char *elt) {
	assert(sp != NULL && elt != NULL); // make sure that both sp & elt exist

	bool found;
	int index = search(sp, elt, &found); // this is the index where the element should be

	return (found) ? sp->elts[index] : NULL; // if elt is found, return elt, otherwise return NULL
}

/*
getElements: allocate and return an array of elements in the set pointed to by sp
runtime: O(n)
*/
char **getElements(SET *sp) {
	assert(sp != NULL); // make sure sp exists

	char **copy = malloc(sizeof(char*)*sp->count); // allocate memory for a copy array
	assert(copy != NULL); // check to make sure memory was allocated

	int i; // i keeps track of the index of the sp array
	int indx = 0; // indx keeps track of the index of the copy array

	for(i = 0; i < sp->length; i ++) { // go through array
		if(sp->flags[i] == FILLED) { // only copy over elements if they have a filled flag
			copy[indx] = strdup(sp->elts[i]); // copy each elt into a copy array
			indx++; // increment index for the copy array
		}
	}

	return copy; // return the new copy array
}
