#define DEBUG 1

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

//Function prototypes
void printUsage();
int printOutput( FILE * stream, t_structNovel * novel, struct hash_table * hash);
FILE * openBook(char * filename);
	
// Functions that manipulate input data from text file
char * getWord(char ** line);

int main(int argc, char **argv)
{	
	int fileIndex = 1, 	// tracks which file we're in
		linecharsread = 0,
		c = 0, // used when getting arguements
		thisbook = 0; // used to specify the order that the books are stored 

	size_t n = 0;
	//t_structWord * allWords = NULL;
	struct structWord allWords;
	char * nextWord = NULL,	
		* nextLine = NULL;
	t_structNovel * novel  = NULL;
	t_structWord * dupWord = NULL; // only used temporarily to store the return value of hash_insert 
					// in order to check the count of the word that was added to determine if its duplicate	
	
	// Check for correct number of arguements
	if (argc != 5)
	{
		printUsage();
		return(0);
	} 
	
	// init novel struct
	novel = initstructNovel();
	if (!novel)
		return -1;

	/a/ get filenames of text files that were provided as arguments
	while (optind < argc -1 )
	{
		// check for options which, when specified, mean the filename of the book follows
		if ((c = getopt(argc, argv, "12")) != -1)
		{
			if (c == '1') // filename of 1st text file
			{
					// attempt to open first text file.

					// because of the way that the output function is set up, we always want drac.txt to be first (that is, if one of the 2 books is drac.txt)
					if (!strcmp(argv[optind], "drac.txt"))
						thisbook = 0;
					else if (!strcmp(argv[optind], "frank.txt"))
						thisbook = 1;
					else 
						thisbook = 0;
					
					novel->book[thisbook] = openBook(argv[optind]);

					if (!novel->book[thisbook])
					{
						// exit gracefully
						novel = destroystructNovel(novel);
						return -1;
					}
					optind++; // optind should be updated to reflect that we found the string that folowed the option that was found
			}

			if (c == '2') // filename of 1st text file
			{
					// attempt to open first text file
					if (!strcmp(argv[optind], "drac.txt"))
						thisbook = 0;
					else if (!strcmp(argv[optind], "frank.txt"))
						thisbook = 1;
					else 
						thisbook = 1;

					novel->book[thisbook] = openBook(argv[optind]);

					if (!novel->book[thisbook])
					{
						//exit gracefully
						novel = destroystructNovel(novel);
						return -1;
					}
					optind++;
			}
		}
	}

	// make sure same book wasn't specified twice
	if (novel->book[0] == novel->book[1])
	{
		#ifdef DEBUG
			printf("Error: you cannot specify the same book twice!\n");
		#endif // DEBUG
		printUsage();
		
		novel = destroystructNovel(novel);
		return -1;
	}

	// make sure 2 book were successfully opened
	if (!novel->book[0] || !novel->book[1])
	{
		printf("Error: Failled to open both books that were specified! terminating...!\n");
		printUsage();
		
		novel = destroystructNovel(novel);
		return -1;
	}

	// initialize the hash table
	struct hash_table * hash = hash_init();

/////////////////Perform Tests/Aggregate Statistics////////////////////////////////////////////////////////////////////
	
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
			//wordcharsread = sscanf(nextLine, "%ms", &nextWord);
			if (nextWord == NULL) // characters found that do not constitute valid "word" (probably control characters \r and/or \n)
			{
			
				linecharsread = 0;	// trigger another line to be read

				free(nextLine);
				nextLine = NULL;
				n = 0;

				continue;
			}

								
			//nextWord = getWord(novel->book[fileIndex-1], novel->linecount[fileIndex - 1]);
			
			else
			{
				// Found valid word
				novel->wordcount[fileIndex-1]++;
				//todo: move this to inside logic to only increment when sure word is unique
				//numwords = novel->uniquewordcount[0] + novel->uniquewordcount[1];	

				// todo: add logic to check for duplicates

/*
				if (numwords - 1 >=  HSZ)	
				// allWords initially has HSZ elements; only resize if necessary
				{
					#ifdef DEBUG
					//printf("numwords - 1 = %d\t", numwords - 1);
					#endif // DEBUG
					allWords = realloc(allWords,
						 (numwords)*sizeof(t_structWord *));
					if (!allWords)
					{
						#ifdef DEBUG
						printf("Fatal error: when resizing allWords, realloc returned NULL! %s\n", strerror(errno));
						#endif // DEBUG
						
						// exit gracefully
						free(nextWord);
						free(nextLine);
						goto destroy;
					}
					allWords[numwords - 1]	= calloc(1, sizeof(t_structWord));
				}
*/

				// init a structWord structure for new word if one doesn't already exist

/*
				allWords = calloc(1, sizeof(t_structWord));
				if (!allWords)
				{
					#ifdef DEBUG
					printf("Fatal error: when resizing allWords, calloc returned NULL!\n");
					#endif // DEBUG
					
					// exit gracefully
					free(nextWord);
					free(nextLine);
					goto destroy;
				}
*/

				strcpy(allWords.word, nextWord);
				// initialize both counts to 0, then overwrite the count of whichever book we are in to reflect that we found this word in particular book
				allWords.count[0] = 0;
				allWords.count[1] = 0;
				allWords.count[fileIndex - 1] = 1;
				
				// get hash address where we'll insert new word into hash table
				//hval = h(nextWord, HSZ);
/*
				if (!hash->table[hval]) 
				// if the linked list at the hash entry doesn't alreadyexists
				{
					hash->table[hval] = list_init();
					if (!hash->table[hval])
{
						#ifdef DEBUG
						printf("Fatal error: list_init() returned NULL!\n");
						#endif // DEBUG
						
						// exit gracefully
						free(nextWord);
						free(nextLine);
						goto destroy;
					}
				}
*/

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
									
				//todo: adapt this to check Hash Table for previous occurrences of words
/*
				// iterate through dictionary file until match of EOF is found
				while (getline(&dictWord, &lenDictWord, dictionaryFile) != -1)
				{
					
					// iterate through each character of current word
					for (y = 0; y < (int)lenDictWord && dictWord[y] != '\0'; y++)
					{
						// make string uppercase
						if (isalpha((unsigned char)dictWord[y]))
							dictWord[y] = toupper(dictWord[y]);
						else if (iscntrl(dictWord[y]))
							dictWord[y] = '\0';
					}
					
					if (dictWord[0] == '\0') // if dictWord is an empty string
					{
						//free (dictWord);
						continue;
					}					
					
					if (!strcmp(nextWord, dictWord))
					{
						dictMatchFnd = 1;
						break;
					}
					
					dictMatchFnd = 0;
				}
				rewind(dictionaryFile);
				
				// If the word was not found in the dictionary, add it to the struct pointer track words not in the dict
				if (!dictMatchFnd)
				{
					novel->numWordsNotInDict++;
					
					novel->ptrWordNotInDict = realloc(novel->ptrWordNotInDict, 
													sizeof(t_wordNotInDict)*novel->numWordsNotInDict);
					
					novel->ptrWordNotInDict[novel->numWordsNotInDict - 1].wordID = novel->numWordsNotInDict - 1;
					
					novel->ptrWordNotInDict[novel->numWordsNotInDict - 1].occurrences = 1;
					
					sscanf(nextWord, "%ms", &novel->ptrWordNotInDict[novel->numWordsNotInDict - 1].strWord);
		
					dictMatchFnd = 0;
				} // end if (!dictMatchFnd)
*/

				//todo: adapt this to aggregate statistics for our purposes
/*				
				// update statistics about occurrences of words of all lengths
				if (len > novel->lenLngstWrd)
					novel->lenLngstWrd = len;
				novel->numWordsOfAllLen[len]++;				
*/
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

	//calculate and output statistics
	if (-1 == printOutput(stdout, novel, hash))
		//todo: print error
		return -1;
	
	destroy:
	// Free and close everything
	//free(nextWord);

/*
	if (allWords)
	{
		for (int j = 0; j < numwords - 1; j++)
		{	
			if (allWords[j])
			{
				if (allWords[j]->word)
				{
					free(allWords[j]->word);
					allWords[j]->word = NULL;
				}
				free(allWords[j]);
				allWords[j] = NULL;
			}
		}
		free(allWords);
	}
*/
	novel = destroystructNovel(novel);
	hash_destroy(hash);
/*
	for (x = 0; x < numFiles; x++)
		fclose(novel->book[x]);
	free(novel->book);
	free(dictWord);
	fclose(outFile);
	fclose(dictionaryFile);
*/
	
	return (0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-
 * parameters:		-
 * Return Value: 	-
 * Assumptions: 	-
 */

void printUsage()
{
	printf("Usage: prog5 -1 First_Text_Name.txt -2 Second_Text_Name.txt\n");
} // end void printUsage()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-fetches next word from file pointer to by ptrFile
 * parameters:		-FILE * ptrFile - file pointer
 * Return Value: 	-on success, returns ptr to string containing next word
 * 					-on failure, returns NULL
 * Assumptions: 	-returned string will be free'd or realloc'ed elsewhere
 */
 
 char * getWord(char ** line)//FILE * ptrFile, long int * linecount)
{
/*
	char * strWord = NULL; // stores next word based on fscanf call (possibly w/ hyphen)
	static char * postStr = NULL; //if a hyphen is found, this contains the portion of the string after the hyphen
									  // and returns it the next time this function is called
	static short wordFound = 0;  	  // boolean, stores whether function needs to return the word store in strNextWord
*/									  // or call fscanf()
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

/*	
	if (wordFound && postStr)
	{
		wordFound = 0;
		if (sscanf(postStr, "%ms", &strWord) == -1)
		{
			// something terrible has happened
			free(strWord);
		}
	}
		

	// Get next white-space delineated string from ptrFile; check for success (correct number of reads)
	if ((result = fscanf(ptrFile, "%ms", &strWord)) != 1)
	{
		// Check for EOF
		if (result == -1 || strWord == NULL)
			return NULL;
			
		// Check for errors
		if (errno == EILSEQ)
			printf("Fatal error within getWord: fscanf() failed: errno == EILSEQ\n");
		else if (errno == EBADF)
			printf("Fatal error within getWord: fscanf() failed: errno == EBADF\n");
		else if (errno == EAGAIN)
			printf("Fatal error within getWord: fscanf() failed: errno == EAGAIN\n"); 
	}
*/	
/*
	if (strchr(strWord, '-'))
	{
		// Replace hyphen with space
		*(strchr(strWord, '-')) = ' ';
		
		result = sscanf(strWord, "%ms %ms", &preStr, &postStr);
		if (preStr == NULL)
			return NULL;
			
		switch (result)
		{
			
			case 2: // 2 words were found, 1 before hyphen, 1 after
			{ 
				wordFound = 1;
				
				preStr = remPunct(preStr);
				postStr = remPunct(postStr);
				
				if (strWord)
					free(strWord);
				return (preStr);
			} // end case 2
			
			case 1: // hyphen found but sscanf only matched 1 word
			{
				//preStr = remPunct(preStr);
				if (strWord)
					free(strWord);
				return (preStr);
			} // end case 1
			default: // something has gone wrong and results of sscanf can't be trusted
			{
				if (strWord)
					free(strWord);
				if (preStr)
					free(preStr);
				if (postStr)
					free(postStr);
				return (NULL);
			} // end case 2
		} // end switch (result)
	} // if (strchr(strWord, '-'))
	else 
		return (remPunct(strWord));	
	return strWord;

*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-removes all punctuation (except for apostrophes [other than those at the start of the word]
 * 							and hyphens)
 * parameters:		-char * punctWord - ptr to string possibly containing punctuation
 * Return Value: 	-on success, returns ptr to string with punctuation removed
 * 					-on failure, returns NULL
 * Assumptions: 	-none
 */
/*
char * remPunct(char * punctWord)
{
	int index = 0, // for 1st level while loop
		index2 = 0, // for nested while loop
		numPunct = 0, // number of punctuation marks found
		len = 0;
		
	char * ptrWord;
	
	if (!punctWord)
		return (NULL);
		
	else if ((len = strlen(punctWord)) == 0)
	{
		printf("Fatal error within remPunct: strlen(punctWord) == 0! terminating...\n");
		if (punctWord)
			free(punctWord);
		return (NULL);
	}
	
	// Remove any initial non-alphabetical characters
	if (!isalpha((unsigned char)punctWord[0]))
	{
		punctWord[0] = ' ';
		
		if (len >= 2)
		{
			if (!isalpha((unsigned char)punctWord[1]))
			{
				punctWord[1] = ' ';
				
				if (len >= 3)
				{
					if (!isalpha((unsigned char)punctWord[2]) && punctWord[2])
						punctWord[2] = ' ';
				}
			}
		}
	}
	
	// if is invalid or consitsted only of punctuation (and is now only spaces)
	if (punctWord == NULL) 
		return NULL;
	else if (len == 1)
	{
		if (!strcmp(punctWord, " "))
		{
			free(punctWord);
			return NULL;
		}
	}
	else if (len == 2)
	{
		if (!strcmp(punctWord, "  "))
		{
			free(punctWord);
			return NULL;
		}
	}
	
	// set index to to first alpha character in punctWord
	index = 0;
	len = strlen(punctWord);
	while (!isalpha((unsigned char)punctWord[index]) && punctWord[index] != '\0' && index < len) {index++;}
	
	//iterate until punctuation is found or string ends; if punct found, remove and shift everything left by 1 position
	for (; index < len && punctWord[index] != '\0'; index++)
	{
		if (ispunct((unsigned char)punctWord[index]))
		{
			// Remove punctuation except '-' and '\''
			if (punctWord[index] != '-' && punctWord[index] != '\'')
			{
				numPunct++;
				for (index2 = index; index2 < len - numPunct; index2++)
				{
					punctWord[index2] = punctWord[index2 + 1];
				}
				// Append '\0'
				punctWord[index2] = '\0';
				index--;
			}
		}	
	}
	
	// again, set positon to first alpha character in punctWord
	index = 0;
	len = strlen(punctWord);
	while (!isalpha((unsigned char)punctWord[index]) && punctWord[index] != '\0' && index < len) {index++;}
	
	// account for special case where punctWord is just a signle '\0' character
	if (punctWord[0] == '\0')
		index = 0;
	
	//copy punctWord (minus leading whitespace) into ptrWord
	ptrWord = calloc(1, strlen(&punctWord[index]) + 1);
	strcpy(ptrWord, &punctWord[index]);
	
	free(punctWord);
	return (ptrWord);
} // end char * remPunct(const char * ptrWord)
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-prints output to file pointed by out, according to specific format
 * parameters:		-t_structNovel * novel - contains all relevant stats about a novel
 * 					-FILE * out - file to output to
 * Return Value: 	-1 on success, 0 on failure
 * Assumptions: 	-none
 */

int printOutput( FILE * stream, t_structNovel * novel, struct hash_table * hash)
{
	if (!stream || ! novel)
		return -1;

	int x = 0,
		y = 0,
		z = 0,
		a = 0,
		book = 0;
	t_structWord * tempWord = NULL;
	char longestwords[10][32]; // used to merge both lists of 5 longest words in each book
	long int longestwordscount[10];
	char mostfreq[30][32]; // used to merge both lists of 15 most freq words
	long int mostfreqcount[30];
	char leastfreq[10][32]; // used to merge both lists of 5 least freq words
	long int leastfreqcount[10];

	// iterate through both books
	for (book = 0; book < 2; book++)
	{
		for (y = 0; y < HSZ; y++)
		{
			if (hash->table[y])
			{
				// to do: add calls to list_next()
				tempWord = list_first(hash->table[y]);

				if (tempWord)
				{
					do {
						// check for longest word in each book
						for (z = 0; z < 5; z++)
						{
							// if this is the first time through the loop, this will be unset
							if (!novel->longestcount[book][z])
							{
								strcpy(novel->longest[book][z], tempWord->word);
								novel->longestcount[book][z] = tempWord->count[book];
								break;
							}
							else 
							{
								if (strlen(tempWord->word) >= strlen(novel->longest[book][z]))
								{
									for (a = 4; a > z; a--)
									{
										strcpy(novel->longest[book][a],
											novel->longest[book][a - 1]);
										novel->longestcount[book][a] = 
											novel->longestcount[book][a - 1];
									}
									strcpy(novel->longest[book][z], tempWord->word);
									novel->longestcount[book][z] = tempWord->count[book];
									break;
								} // end else if
							}

						} // end for (z = 0; z < 5; z++)

						// check for 15 most used words in each book
						for (z = 0; z < 15; z++)
						{
							if (!novel->mostusedcount[book][z])
							{
								strcpy(novel->mostused[book][z], tempWord->word);
								novel->mostusedcount[book][z] = tempWord->count[book];
								break;
							}
							else if (tempWord->count[book] 
									>= novel->mostusedcount[book][z])
							{
								for (a = 14; a > z; a--)
								{
									strcpy(novel->mostused[book][a],
										 novel->mostused[book][a - 1]);
									novel->mostusedcount[book][a] = 
										novel->mostusedcount[book][a - 1];
								}
								strcpy(novel->mostused[book][z], tempWord->word);
								novel->mostusedcount[book][z] = tempWord->count[book];
								break;
							} // end else if

						} // end for (z = 0; z < 5; z++)

						// check for 5 least used
						if (tempWord->count[book] != 0)
						{
							for (z = 0; z < 5; z++)
							{
								if (!novel->leastusedcount[book][z])
								{
									strcpy(novel->leastused[book][z], tempWord->word);
									novel->leastusedcount[book][z] = tempWord->count[book];
									break;
								}
								else if (tempWord->count[book] 
										<= novel->leastusedcount[book][z])
								{
									for (a = 4; a > z; a--)
									{
										strcpy(novel->leastused[book][a],
											 novel->leastused[book][a - 1]);
										novel->leastusedcount[book][a] = 
											novel->leastusedcount[book][a - 1];
									}
									strcpy(novel->leastused[book][z], tempWord->word);
									novel->leastusedcount[book][z] = tempWord->count[book];
									break;
								} // end else if

							} // end for (z = 0; z < 5; z++)

							// determine if this word appears in both books
							if (tempWord->count[0] >= 1 && tempWord->count[1] >= 1)
							{
								novel->wordinbothcount++;
							}

							// calculate the number of unique words in both books combined
							if (tempWord->count[0] + tempWord->count[1] == 1)
								novel->uniquewordinbothcount++;
						}

						tempWord = list_next(hash->table[y]);

					} while (tempWord);
					
				} // end if if (tempWord)

			}	// end if (hash->table[y])

		} // end for (int y = 0; y < HSZ; y++)

	} // end for (int book = 0; b < 1; book++)

	// merge both lists of 5 longest words into one list, and retrieve the 5 longest from that list
	// longestwords is just an intermediate array to store the merged lists (as is longestwordscount)
	for (y = 0; y < 10; y++)
	{
		if (y < 5)
		{
			strcpy(longestwords[y], novel->longest[0][y]);
			longestwordscount[y] = novel->longestcount[0][y];
		}
		else
		{
			strcpy(longestwords[y], novel->longest[1][y - 5]);
			longestwordscount[y] = novel->longestcount[1][y - 5];
		}
			
	}

	int indexoflongest = 0;

	for (x = 0; x < 5; x++)
	{
		for (y = 0; y < 10; y++)
		{
			if (strlen(longestwords[indexoflongest]) < strlen(longestwords[y]))
			{
				indexoflongest = y;
			}
		}
		strcpy(novel->longestinboth[x], longestwords[indexoflongest]);
		novel->longestinbothcount[x] = longestwordscount[indexoflongest];
		strcpy(longestwords[indexoflongest], "");	// this prevents this value from being added as a duplicate next time the loop executes
		indexoflongest = 0;
	}

	// merge both lists of 15 most freq words into one list, and retrieve the 15 most freq of that set of 30
	// (similar to above proc. for 5 longest)
	for (y = 0; y < 30; y++)
	{
		if (y < 15)
		{
			// the number of occurrences[frequency] of a word is the number of occurrences across both books
			strcpy(mostfreq[y], novel->mostused[0][y]);
			mostfreqcount[y] = novel->mostusedcount[0][y];
		}
		else
		{
			strcpy(mostfreq[y], novel->mostused[1][y - 15]);
			mostfreqcount[y] = novel->mostusedcount[1][y - 15];
		} // end for (y = 0
	} // end of for(y = 0; y < 30; y++)

	// iterate through list and check if this word is a duplicate
	for (y = 0; y < 30; y++)
	{
		// pick a word; starting from thar word's position in list, search through rest of list for more occurrences
		// of that word
		for (z = y + 1; z < 30; z++)
		{
			if (!strcmp(mostfreq[y], mostfreq[z]))	// found duplicate
			{
				mostfreqcount[y] += mostfreqcount[z];
				strcpy(mostfreq[z], "");
				mostfreqcount[z] = 0;
			} // end if (!strcmp
		} // end for (z = y;

	} // end for (y = 0l y < 30; y++)


	int indexofmostfreq = 0;

	for (x = 0; x < 15; x++)
	{
		for (y = 0; y < 30; y++)
		{
			if (mostfreqcount[indexofmostfreq] < mostfreqcount[y])
			{
				indexofmostfreq = y;
			}
		}

		strcpy(novel->mostusedinboth[x], mostfreq[indexofmostfreq]);
		novel->mostusedinbothcount[x] = mostfreqcount[indexofmostfreq];
		mostfreqcount[indexofmostfreq] = 0;	// this prevents this value from being added as a duplicate next time the loop executes
		indexofmostfreq = 0;
	}

	// merge both lists of 5 least freq words into one list, and retrieve the 5 least freq of that set of 10
	// (similar to above proc. for 15 most freq)
	for (y = 0; y < 10; y++)
	{
		if (y < 5)
		{
			// the number of occurrences[frequency] of a word is the number of occurrences across both books
			strcpy(leastfreq[y], novel->leastused[0][y]);
			leastfreqcount[y] = novel->leastusedcount[0][y];
		}
		else
		{
			strcpy(leastfreq[y], novel->leastused[1][y - 5]);
			leastfreqcount[y] = novel->leastusedcount[1][y - 5];
		} // end for (y = 0
	} // end of for(y = 0; y < 30; y++)

	// iterate through list and check if this word is a duplicate
	for (y = 0; y < 10; y++)
	{
		// pick a word; starting from thar word's position in list, search through rest of list for more occurrences
		// of that word
		for (z = y + 1; z < 10; z++)
		{
			if (!strcmp(leastfreq[y], leastfreq[z]))	// found duplicate
			{
				leastfreqcount[y] += leastfreqcount[z];
				strcpy(leastfreq[z], "");
				leastfreqcount[z] = 9999;
			} // end if (!strcmp
		} // end for (z = y;

	} // end for (y = 0l y < 10; y++)


	int indexofleastfreq = 0;

	for (x = 0; x < 5; x++)
	{
		for (y = 0; y < 10; y++)
		{
			if (leastfreqcount[indexofleastfreq] > leastfreqcount[y])
			{
				indexofleastfreq = y;
			}
		}

		strcpy(novel->leastusedinboth[x], leastfreq[indexofleastfreq]);
		novel->leastusedinbothcount[x] = leastfreqcount[indexofleastfreq];
		leastfreqcount[indexofleastfreq] = 9999;	// this prevents this value from being added as a duplicate next time the loop executes
		indexofleastfreq = 0;
	}



/////////////////////////////////// PRINT STATISTICS ///////////////////////////////////////////

	fprintf(stream, "\nECE2230 Fall 2017 Text Hashing Project\n");	
	fprintf(stream, "Adam Van Scyoc\n");	
	fprintf(stream, "\nDracula\n\n");	
	fprintf(stream, "Number of lines processed\t\t%-8ld\n", novel->linecount[0]);	
	fprintf(stream, "Number of Words processed\t\t%-8ld\n", novel->wordcount[0]);	
	fprintf(stream, "Number of Unique words\t\t\t%-8ld\n", novel->uniquewordcount[0]);	

	// Book 1 Stats
	fprintf(stream, "\n5 largest words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%2ld\n", novel->longest[0][x], novel->longestcount[0][x]);	

	fprintf(stream, "\n15 most frequently used words\n");	
	for (x = 0; x < 15; x++)
		fprintf(stream, "\t%-24s\t%2ld\n", novel->mostused[0][x], novel->mostusedcount[0][x]);	
	
	fprintf(stream, "\n5 least frequently used words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%2ld\n", novel->leastused[0][x], novel->leastusedcount[0][x]);	

	fprintf(stream, "\nFrankenstein\n\n");	
	fprintf(stream, "Number of lines processed\t\t%-8ld\n", novel->linecount[1]);	
	fprintf(stream, "Number of lines processed\t\t%-8ld\n", novel->wordcount[1]);	
	fprintf(stream, "Number of Unique words\t\t\t%-8ld\n", novel->uniquewordcount[1]);	

	// Book 2 Stats
	fprintf(stream, "\n5 largest words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->longest[1][x], novel->longestcount[1][x]);	

	fprintf(stream, "\n15 most frequently used words\n");	
	for (x = 0; x < 15; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->mostused[1][x], novel->mostusedcount[1][x]);	
	
	fprintf(stream, "\n5 least frequently used words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->leastused[1][x], novel->leastusedcount[1][x]);	

	// Overall Stats
	fprintf(stream, "\nCommon Vocabulary (appears in both)\n");	
	fprintf(stream, "\nNumber of Unique Words\t\t\t%ld\n", novel->uniquewordinbothcount);	
	fprintf(stream, "\n 5 Largest common words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->longestinboth[x], novel->longestinbothcount[x]);	

	fprintf(stream, "\n15 most frequently used common words\n");	
	for (x = 0; x < 15; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->mostusedinboth[x], novel->mostusedinbothcount[x]);	
	fprintf(stream, "\n5 Least Frequently used common words\n");	
	for (x = 0; x < 5; x++)
		fprintf(stream, "\t%-24s\t%ld\n", novel->leastusedinboth[x], novel->leastusedinbothcount[x]);	

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FILE * openBook(char * filename)
{

	FILE * novel = NULL;

	if (0 != access(filename, R_OK)) // file inaccessible
	{
		#ifdef DEBUG
		printf("Fatal Error: attempt to open %s failed! terminating...\n%s\n", filename, strerror(errno));
		#endif
		printUsage();
		return (NULL);
	}
/*
	// get number of files by testing if program can access them
	do {
		// If searching for first file (loop running for first time), 
		// test for special case where file may have either suffix _0.txt or _1.txt
		if (numFiles == 0)
		{
			sprintf(inpPath, "%s_1.txt", argv[1]);
			
			// check for filename_1.txt
			sprintf(inpPath, "%s_1.txt", argv[1]);
			if (!access(inpPath, R_OK))
			{
				numFiles = 1;
				fileIndex = 1;
				continue;
			} // end if (!access(inpPath, R_OK))
			else // found no files; inpPath invalid
			{
				numFiles = 0;
				break;
			} // end else (!access(inpPath, R_OK))	
		} // if (numFiles == 0)
		
		sprintf(inpPath, "%s_%d.txt", argv[1], numFiles);
		
		if (!access(inpPath, R_OK)) // Possibility of more files
			numFiles++;
			
		else // Last file found 
		{
			numFiles--;
			break;
		}
	} while (1);
*/	
	
	// attempt to open file
	novel = fopen(filename, "r");
	if (!novel)
	{
		printf("Fatal Error: attempt to open %s failed! terminating...\n", filename);
		return NULL;
	}

	return novel;

/*
	// allocate space for array of FILE pointers (one per text file)
	novel->book = (FILE **)calloc(sizeof(FILE), numFiles);
	
	// open input files, establish FILE pointer to each
	for (x = 0; x < numFiles; x++)
	{
		sprintf(inpPath, "%s_%d.txt", argv[1], x+1);
		
		// attempt to open file, check for success
		novel->book[x] = fopen(inpPath, "r");
		if (!novel->book[x])
		{
			printf("Fatal Error: attempt to open %s failed! terminating...\n", inpPath);
			
			// close open file pointers (if any)
			if (x > 0)
			{
				while (--x) { fclose(novel->book[x]); }
			} // end if (x > 0)
			
			free (novel->book);
			return (0);
		} // if (!novel->book[x])
					
	} // end for (int x = 0; x < index; x++)
	
	// open output.txt, check success
		// Check if argument 2 includes the file suffix, otherwise add it
	if (strstr(argv[2], ".txt"))
		sprintf(outPath, "%s", argv[2]);
	else
		sprintf(outPath, "%s.txt", argv[2]);
	outFile = fopen(outPath, "w+");
	if (!outFile)
	{
		printf("Fatal Error: attempt to open %s failed! terminating...\n", argv[2]);
		
		for (x = 0; x < numFiles; x++)
		{
			fclose(novel->book[x]);
		} // end for (int x = 0; x < numFiles; x++)
		
		free (novel->book);
		return (0);
	} // end if (!outFile)
	
	// open dictionary.txt
	dictionaryFile = fopen("dictionary.txt", "r");
	if (!dictionaryFile)
	{
		printf("Fatal Error: attempt to open dictionary.txt failed! terminating...\n");
		
		for (x = 0; x < numFiles; x++)
		{
			fclose(novel->book[x]);
		} // end for (int x = 0; x < numFiles; x++)
		
		free (novel->book);
		fclose(outFile);
		return (0);
	}
*/
}

