/* COEN 12 Project 5 - File: huffman.c
 * Author: Madeleine Waldie
 * Due Date: 5/30/21
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h" // where node struct is defined

#define NUM_ENTRIES 257 // nodes and count arrays need to contain 257 entries, with the last entry being the end of file character

/*
   makeNode (two purposes): 1. Create a new node with the given data. 
   2. Create a new node with the given data and set the parent of the left and right node passed in equal to the newly created node
runtime: O(1)
*/

struct node* makeNode(int data, struct node* left, struct node* right) {
	struct node* node = malloc(sizeof(struct node*)); // allocate memory for node
	assert(node != NULL); // make sure node exists

	node->parent = NULL;
	node->count = data;

	if((left!= NULL) && (right != NULL)) {
		left->parent = node;
		right->parent = node;
	}

	return node;	
}

/* 
depth: returns the depth of a given node
runtime: O(1)
*/

int depth(struct node* child) { 
	if (child->parent == NULL) { // base case - zero depth
		return 0;
	}

	return (depth(child->parent)+1); // recursively call depth
}

/*
nodeCompare: comparison function for two given nodes that you pass to your createQueue
runtime: O(1)
*/

int nodeCompare(struct node* left, struct node* right) {
	if(left->count < right->count) {
		return -1;
	}
	if(left->count > right->count) {
		return 1;
	}
	return 0;	
}

// main function: following steps from lab handout
int main(int argc, char*argv[])
{
	assert(argc == 3); // make sure appropriate # of arguments passed
	int i, j, k, c, bits, depthNode, charLoc; // variables used throughout code

	/* Step 1: Count the number of occurrences of each character in the file. Keep track of these counts in a count array */
	int counts[NUM_ENTRIES]; // array for num of chars in file
	struct node *nodes[NUM_ENTRIES];	

	//initializing the empty tree
	for(k = 0; k < NUM_ENTRIES;k++)
	{
		counts[k] = 0;
	}

	FILE* file = fopen(argv[1],"r"); //read from input.txt

	if(file == NULL) { // exit if file doesn't have any data
		printf("Error. File is null.\n");
		return -1;
	}

	while((charLoc = getc(file)) != EOF) { // loop through file till end of file
		counts[charLoc]++; // increase count of chars
	}
	fclose(file);

	/* Step 2: Create a binary tree consisting of just a leaf for each character with a nonzero count. 
	   The data for the tree is the character’s count. Also, create a tree with a zero count for the end of file marker. 
	   Insert all trees into a priority queue, and also store each node in an array similar to the count array. */

	PQ *pq = createQueue(nodeCompare); // create a priority queue

	//create leaf nodes for each char
	for (c = 0; c < NUM_ENTRIES - 1; c++) { // loop through number of characters
		if(counts[c] > 0) { // only if it has a nonzero count
			nodes[c] = makeNode(counts[c], NULL, NULL); //create a leaf for character
			addEntry(pq, nodes[c]); // add leaf node to priority queue
		}
		else {
			nodes[c] = NULL; //otherwise, set nodes[c] to null
		}
	}
	nodes[NUM_ENTRIES - 1] = makeNode(0, NULL, NULL); // tree with zero count for end of file marker
	addEntry(pq, nodes[NUM_ENTRIES - 1]); // add to priority queue

	/* Step 3: While the priority queue has more than one tree in it, remove the two lowest priority trees. 
	   Create a new tree with each of these trees as subtrees. The count for the new tree is the sum of the counts of the two subtrees.
	   Insert the new tree into the priority queue */

	// nodes used in while loop below
	struct node*n1;
	struct node*n2;
	struct node*np;

	//add to the tree
	while(numEntries(pq) > 1) { //pq has more than one node
		n1 = removeEntry(pq); // first node removed of lowest priority trees
		n2 = removeEntry(pq); // second node removed
		i = n1->count + n2->count;
		np = makeNode(i, n1, n2); // the new node created that will have its count as the count of its two children (the ones we just removed)
		addEntry(pq, np); // add that new node back into the priority queue
	}

	/* Step 4: Eventually, there will be only one tree remaining in the priority queue. This is the Huffman tree. Incidentally,
	   the data at the root of this tree should equal the number of characters in the file. 
	   Print out information about all the characters and call pack() */

	for(j = 0; j < NUM_ENTRIES; j++) { //print out the number of bits, following format in lab doc
		if(nodes[j] != NULL) { // as long as node exists
			depthNode = depth(nodes[j]); // compute depth
			bits = counts[j] * depthNode; // compute num bits
			(isprint(j) == 0) ? printf("%o", j) : printf("'%c'",j); //  using the isprint function declared in<ctype.h> to determine if a character is printable
			printf(": %d x %d bits = %d bits\n", counts[j], depthNode, bits); // print depth x bits
		} 
	}

	pack(argv[1], argv[2], nodes); // Call pack() at the end of main; 

	return 0; // exit program

}
