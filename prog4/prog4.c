/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "hash.h"
#include "novel.h"
#include "list.h"
#include "fileio.h"

char * getWord(char ** line); // when passed a string that contains a line of text, retrieves the next word from that line and removes that text from the line


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{	
	int fileIndex = 1, 	// tracks which file we're in
		linecharsread = 0;

	size_t n = 0;
	//t_structWord * allWords = NULL;
	struct structWord allWords;
	char * nextWord = NULL,	
		* nextLine = NULL;
	t_structNovel * novel  = NULL;
	t_structWord * dupWord = NULL; // only used temporarily to store the return value of hash_insert 
					// in order to check the count of the word that was added to determine if its duplicate	
	

///////////////// Initializations and Startup ////////////////////////////////////////////////////////////////////

	// Check for correct number of arguements
	if (argc != 5)
	{
		printUsage();
		return(-1);
	} 
	
	// init novel struct
	novel = initstructNovel();
	if (!novel)
		return -1;

	// get command lines argumentes and attempt to open spefied books
	if (-1 == getargs(argc, argv, novel))
	{
		// freeing of data and error messages taken care of by getargs
		return -1;
	}

	// initialize the hash table
	struct hash_table * hash = hash_init();

///////////////// Perform Tests/Aggregate Statistics ////////////////////////////////////////////////////////////////////
	
	// Iterate through each input file
	do 
	{
		// Iterate word-by-word
		while (!feof(novel->book[fileIndex-1]))
		{
			if (linecharsread <= 0)
			{
				// loop till next non-empty line
				free(nextLine);
				linecharsread = getline(&nextLine, &n, novel->book[fileIndex-1]);

				// reached end of file or eerror
				if (linecharsread == -1)
				{
					if (nextLine)
					{
						free(nextLine);
						nextLine = NULL;
						n = 0;
					}
			
					continue;
				}
				else					// a valid line was found; increment counter
					novel->linecount[fileIndex-1]++;

				// strip carriage return and newline characters and recompute the number of characters in the line
				if (strstr(nextLine, "\r\n"))	
				{
					*strstr(nextLine, "\r\n") = '\0';
					linecharsread -= 2;
					if (linecharsread <= 0)
					{
						if (nextLine)
						{
							free(nextLine);
							nextLine = NULL;
							n = 0;
						}
				
						continue;
					}
				}
			}
			
			// continue fetching words from 
			// previously read-in line
			nextWord = getWord(&nextLine);
			if (nextWord == NULL) // characters found that do not constitute valid "word" (probably control characters \r and/or \n)
			{
			
				linecharsread = 0;	// trigger another line to be read

				free(nextLine);
				nextLine = NULL;
				n = 0;

				continue;
			}

			else
			{
				// Found valid word
				novel->wordcount[fileIndex-1]++;

				strcpy(allWords.word, nextWord);

				// initialize both counts to 0, then overwrite the count of whichever book we are in to reflect that we found this word in particular book
				allWords.count[0] = 0;
				allWords.count[1] = 0;
				allWords.count[fileIndex - 1] = 1;
				
				// attempt to insert new word into hash table
				// save the return value to check if the word we added was unique
				dupWord = hash_insert(hash, nextWord, allWords, fileIndex);

				if (!dupWord)	
				{
						#ifdef DEBUG
						printf("Fatal error: hash_insert() returned NULL!\n");
						#endif // DEBUG
						
						// exit gracefully
						free(nextWord);
						free(nextLine);
						goto destroy;
				}

				else if (1 == dupWord->count[0] + dupWord->count[1]) // a completely unique word only 
											// occurs once across both books
				{
					novel->wordinbothcount++;
				}

				// if word is unique relative to individual books, increment appropriate counters
				if (1 == dupWord->count[0])
					novel->uniquewordcount[0]++;
				if (1 == dupWord->count[1])
					novel->uniquewordcount[1]++;

			} // end else (nextWord)
					
			free(nextWord);
			nextWord = NULL;

		} // end while (!feof(novel->book[fileIndex]))
		
		// If there's another file, iterate to next file
		if (fileIndex == 1)
			fileIndex++;
		else 
			break;
			
	} while (1);


///////////////// Calculate and Output Statistics ////////////////////////////////////////////////////////////////////

	if (-1 == printOutput(stdout, novel, hash))
	{
		printf("Fatal Error: Failled when attempting to calculate and output results! ..terminating\n");
		goto destroy;
	}

///////////////// Destroy/De-initialize Everything ////////////////////////////////////////////////////////////////////

	destroy:

	novel = destroystructNovel(novel);
	hash_destroy(hash);
	
	return (0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// fetches next word from file pointer to by ptrFile
char * getWord(char ** line)//FILE * ptrFile, long int * linecount)
{
	int result = 0;
	char * word = NULL,
		* tempLine = NULL;
									  
	if (!*line)
		return(NULL);
	
	result = sscanf(*line, "%ms", &word);
	if (result == -1 && ferror(stdin))
	{
		#ifdef DEBUG
		printf("Error trying to scan in neext word from current line! %s\n", 
			strerror(errno));
		#endif
		return NULL;
	}
	else if (!word)
		return NULL;

	tempLine = calloc(1, sizeof(char)*strlen(&strstr(*line, word)[strlen(word) - 1]));
	
	strcpy(tempLine, &strstr(*line, word)[strlen(word)]);
	
	// resize line to reflect the characters that we've removed off of the front of it
	// so that next search will return the next word (not the same word again)
	//*line = realloc(line, strlen(tempLine) + 1);
	strcpy(*line, tempLine);

	free(tempLine);

	return word;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
