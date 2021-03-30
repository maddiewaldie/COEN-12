/* COEN 12 Lab #1
 * Author: Madeleine Waldie
 * Due Date: 4/4/21
 * 
 * Count number of words in a text file 
 * (sample path to text file: /scratch/coen12/FILENAME.txt)
 */

#include <stdio.h>
#define MAX_WORD_LENGTH 30

int main(int numArgs, char*args[]) {

	char word[MAX_WORD_LENGTH+1]; // Allocate memory to store word in a char array
	int numWords = 0; // Variable to count number of words

	FILE *file = fopen(args[1], "r"); // Initialize pointer to file & open it

	// If person doesn't input a file name, or if file name doesn't exist, send message to user to try again
	if (numArgs == 1) {
		printf("File name is missing. Please input a file name in order to determine how many words it contains. \n");
		return 0;
	}
	else if (file == NULL) {
		printf("File does not exist. Please input a different file name in order to determine how many words it contains. \n");
		return 0;
	}

	// Go through file & increment number of words
	while(fscanf(file,"%s", word) > 0) {	
		numWords++;
	}

	fclose(file); // Close file
	printf("%i total words\n", numWords); // Print number of words
	return 0;
}	
