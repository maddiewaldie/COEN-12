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

	FILE* file = fopen(argv[1],"c"); //read from input.txt
	
	if(file == NULL) { // exit if file doesn't have any data
		printf("Error\n");
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
	for (c = 0; c < NUM_ENTRIES - 1; c++) {
		if(counts[c]>0) {
			nodes[c] = makeNode(counts[c], NULL, NULL);
			addEntry(pq, nodes[c]);
		}
		else {
			nodes[c] = NULL;
		}
	}
	nodes[NUM_ENTRIES - 1] = makeNode(0, NULL, NULL);
	addEntry(pq, nodes[NUM_ENTRIES - 1]);
	
    /* Step 3: While the priority queue has more than one tree in it, remove the two lowest priority trees. 
    Create a new tree with each of these trees as subtrees. The count for the new tree is the sum of the counts of the two subtrees.
    Insert the new tree into the priority queue */
	
	struct node*n1;
	struct node*n2;
	struct node*np;
	
	//add to the tree
	while(numEntries(pq) > 1) { //pq has more than one node
		n1 = removeEntry(pq);
		n2 = removeEntry(pq);
		i = n1->count + n2->count;
		np = makeNode(i, n1, n2);
		addEntry(pq, np);
	}

    /* Step 4: Eventually, there will be only one tree remaining in the priority queue. This is the Huffman tree. Incidentally,
    the data at the root of this tree should equal the number of characters in the file. 
    Print out information about all the characters and call pack() */

	for(j = 0; j < NUM_ENTRIES; j++) { //print out the number of bits
		if(nodes[j]!=NULL) {
			depthNode = depth(nodes[j]);
			bits = counts[j] * depthNode;
			(isprint(j) == 0) ? printf("%o", j) : printf("'%c'",j);
			printf(": %d x %d bits = %d bits\n", counts[j], depthNode, bits);
		} 
	}

	pack(argv[1], argv[2], nodes); // Call pack() at the end of main; 

	return 0;
    
}