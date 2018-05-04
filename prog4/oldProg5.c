/*NAME: 			ADAM VAN SCYOC
 *CLEMSON LOGIN:	AVANSCY
 *COURSE #:			ECE 2220 001
 *SEMESTER:			SPRING 2017
 *ASSIGNMENT:		PROGRAM 5
 *
 *PURPOSE: THIS IS THE SOLE FILE FOR PROGRAM5, WHICH ACCEPTS A S INPUT A CHARACTER STRING THAT IS THE TITLE OF A NOVEL,
 * 			AS WELL AS THE NAME OF AN OUTPUT FILE (BOTH OF WHICH ARE TEXT FILES) AND AGGREGATES VARIOUS DATA ABOUT THE NOVEL
 *
 *ASSUMPTIONS: -INPUT FILE CONTAINS ONLY VALID ASCII CHARACTERS (NOT UNICODE OR ASCII-EXTENDED)
 * 				-IF OUTPUT FILE SPECIFIED DOES NOT EXIST, IT WILL BE CREATED; IF IT EXISTS, IT WILL BE TRUNCATED
 * 					TO ZERO
 * 				-NONE OF THE FILES INVOLVED ARE LOCKED AND ALL HAVE PROPER PERMISSIONS
 * 				-PERSON EXECUTING THIS PROGRAM HAS SOME TIME ON THEIR HANDS
 * 
 * KNOWN BUGS: NONE - VALGRIND REPORTS NO MEMORY LEAKS, NO DYNAMIC MEMORY ALLOCATION BUGS
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

//Typedefs
	// for statistics abouts words found that are not in dictionary.txt
typedef struct wordNotInDict
{
	char * strWord;
	short occurrences;
	short wordID;
} t_wordNotInDict, * t_ptrWordNotInDict;
	
	// for statistics about containing the string "OUS"
typedef struct wordOUS
{
	char * strWord;
	short occurrences;
	short wordID;
} t_wordOUS, * t_ptrwordOUS;

	// for statistics about containing the string "OUS"
typedef struct wordACESLMTW
{
	char * strWord;
	short occurrences;
	short wordID;
} t_wordACESLMTW, * t_ptrwordACESLMTW;

	// for statistics about individual letters
typedef struct letters
{
	int occurrences;
	float frequency;
	
} t_letters, * t_ptrletters;

	// statistics about each text file of a novel
typedef struct structNovel {
	int totalLetters;
	t_letters letterStats[26];
	int numWords;
	int lenLngstWrd;
	int numWordsOfAllLen[32];
	
	
	short numWordsOUS;
	t_ptrwordOUS ptrWordsOUS;
	
	short numWordsACESLMTW;
	t_ptrwordACESLMTW ptrWordsACESLMTW;
	
	short numWordsNotInDict;
	t_ptrWordNotInDict ptrWordNotInDict;
} t_structNovel, * t_ptrstructNovel;

//Function prototypes
void printUsage();
int printOutput(t_ptrstructNovel ptrNovel, FILE * out);
	
	// This function accepts t_ptrwordACESLMTW as arguement, but works equally well for t_ptrwordOUS or t_NotInDict
t_ptrwordACESLMTW remDuplicatesWords(t_ptrwordACESLMTW ptrWordACESLMTW, int numWordsACESLMTW);

	// Functions that manipulate input data from text file
char * getWord(FILE * ptrFile);
char * remPunct(char * ptrWord);

	//Structure constructors/destructors
t_ptrstructNovel initstructNovel();
t_ptrstructNovel destroystructNovel(t_ptrstructNovel);

int main(int argc, char **argv)
{	
	int numFiles = 0,
		x = 0,
		y = 0,
		fileIndex = 0,
		len = 0,
		dictMatchFnd = 0;
	unsigned long lenDictWord = 0l;
	FILE * outFile = NULL,
		 * dictionaryFile = NULL,
		 ** inpFile  = NULL;
	char inpPath[64],
		 outPath[64];
	char * nextWord = NULL,
		 * dictWord = NULL; // contains next word from dictionary.txt for comparison
	t_ptrstructNovel ptrNovel  = NULL;
	
	
////Initialization/Open File Streams//////////////////////////////////////////////////////////////////////////////////
	
	// Check for correct number of arguements and that 2nd arg. contains no whitespace
	if (argc != 3 || strchr(argv[1], 32) )
	{
		printUsage();
		return(0);
	} // end if (argc != 3 || strchr(argv[1], 32) )
		
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
	
	// if argv[1] is not a valid novel name
	if (numFiles <= 0)
	{
		printf("Fatal Error: attempt to open %s failed! terminating...\n", inpPath);
		printUsage();
		return (0);
	}
	
	// allocate space for array of FILE pointers (one per text file)
	inpFile = (FILE **)calloc(sizeof(FILE), numFiles);
	
	// open input files, establish FILE pointer to each
	for (x = 0; x < numFiles; x++)
	{
		sprintf(inpPath, "%s_%d.txt", argv[1], x+1);
		
		// attempt to open file, check for success
		inpFile[x] = fopen(inpPath, "r");
		if (!inpFile[x])
		{
			printf("Fatal Error: attempt to open %s failed! terminating...\n", inpPath);
			
			// close open file pointers (if any)
			if (x > 0)
			{
				while (--x) { fclose(inpFile[x]); }
			} // end if (x > 0)
			
			free (inpFile);
			return (0);
		} // if (!inpFile[x])
					
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
			fclose(inpFile[x]);
		} // end for (int x = 0; x < numFiles; x++)
		
		free (inpFile);
		return (0);
	} // end if (!outFile)
	
	// open dictionary.txt
	dictionaryFile = fopen("dictionary.txt", "r");
	if (!dictionaryFile)
	{
		printf("Fatal Error: attempt to open dictionary.txt failed! terminating...\n");
		
		for (x = 0; x < numFiles; x++)
		{
			fclose(inpFile[x]);
		} // end for (int x = 0; x < numFiles; x++)
		
		free (inpFile);
		fclose(outFile);
		return (0);
	}
	
/////////////////Perform Tests/Aggregate Statistics////////////////////////////////////////////////////////////////////
	
	ptrNovel = initstructNovel();
	
	// Iterate through each input file
	do 
	{
		// Iterate word-by-word
		while (!feof(inpFile[fileIndex-1]))
		{
			nextWord = getWord(inpFile[fileIndex-1]);
			if (nextWord == NULL)
				continue;
			
			else
			{
				// Found valid word
				ptrNovel->numWords++;
				len = strlen(nextWord);
				
				// iterate through each character of current word
				for (y = 0; y < len; y++)
				{
					// adjust relevant statistics
					if (isalpha(nextWord[y]))
					{
						nextWord[y] = toupper(nextWord[y]);
						
						ptrNovel->totalLetters++;
						ptrNovel->letterStats[toupper(nextWord[y])-65].occurrences++;
					}
				}
					
				// Test if string contains substring "OUS"
				if (strstr(nextWord, "OUS"))
				{
					
					ptrNovel->numWordsOUS++;
					
					// add element to vector containing series of t_wordOUS structs tracking words with OUS
					ptrNovel->ptrWordsOUS = realloc(ptrNovel->ptrWordsOUS, 
						(sizeof(t_wordOUS)*ptrNovel->numWordsOUS));
						
					ptrNovel->ptrWordsOUS[ptrNovel->numWordsOUS - 1].wordID = ptrNovel->numWordsOUS - 1;
					
					ptrNovel->ptrWordsOUS[ptrNovel->numWordsOUS - 1].occurrences = 1;
						
					sscanf(nextWord, "%ms", &ptrNovel->ptrWordsOUS[ptrNovel->numWordsOUS - 1].strWord);
					
				} // if (strstr(nextWord, "OUS"))
				
				// Test if string consists only of letters ACESLMTW
				else if (strspn(nextWord, "ACESLMTW") == len)
				{						
					
					ptrNovel->numWordsACESLMTW++;

					ptrNovel->ptrWordsACESLMTW = realloc(ptrNovel->ptrWordsACESLMTW,
						sizeof(t_wordACESLMTW)* ptrNovel->numWordsACESLMTW);
						
					ptrNovel->ptrWordsACESLMTW[ptrNovel->numWordsACESLMTW - 1].wordID = ptrNovel->numWordsACESLMTW - 1;
					
					ptrNovel->ptrWordsACESLMTW[ptrNovel->numWordsACESLMTW - 1].occurrences = 1;
							
					sscanf(nextWord, "%ms", &ptrNovel->ptrWordsACESLMTW[ptrNovel->numWordsACESLMTW - 1].strWord);
					
				} // end if (strspn(nextWord, "ACESLMTW") == len) 
				
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
					ptrNovel->numWordsNotInDict++;
					
					ptrNovel->ptrWordNotInDict = realloc(ptrNovel->ptrWordNotInDict, 
													sizeof(t_wordNotInDict)*ptrNovel->numWordsNotInDict);
					
					ptrNovel->ptrWordNotInDict[ptrNovel->numWordsNotInDict - 1].wordID = ptrNovel->numWordsNotInDict - 1;
					
					ptrNovel->ptrWordNotInDict[ptrNovel->numWordsNotInDict - 1].occurrences = 1;
					
					sscanf(nextWord, "%ms", &ptrNovel->ptrWordNotInDict[ptrNovel->numWordsNotInDict - 1].strWord);
		
					dictMatchFnd = 0;
				} // end if (!dictMatchFnd)
				
				// update statistics about occurrences of words of all lengths
				if (len > ptrNovel->lenLngstWrd)
					ptrNovel->lenLngstWrd = len;
				ptrNovel->numWordsOfAllLen[len]++;				
			} // end else (nextWord)
					
		free(nextWord);
		} // end while (!feof(inpFile[fileIndex]))
		
		// If there's another file, iterate to next file
		if (fileIndex < numFiles)
			fileIndex++;
		else 
			break;
			
	} while (1);
	//} while (fileIndex++ < numFiles);
	
	// Remove duplicates in vectors of words
	ptrNovel->ptrWordsOUS = (t_ptrwordOUS) remDuplicatesWords((t_ptrwordACESLMTW)ptrNovel->ptrWordsOUS, 
																		ptrNovel->numWordsOUS);
	ptrNovel->ptrWordsACESLMTW = remDuplicatesWords(ptrNovel->ptrWordsACESLMTW, ptrNovel->numWordsACESLMTW);
	ptrNovel->ptrWordNotInDict = (t_ptrWordNotInDict) remDuplicatesWords((t_ptrwordACESLMTW)ptrNovel->ptrWordNotInDict,
																		ptrNovel->numWordsNotInDict);
	
	printOutput(ptrNovel, outFile);
	
	// Free and close everything
	ptrNovel = destroystructNovel(ptrNovel);
	for (x = 0; x < numFiles; x++)
		fclose(inpFile[x]);
	free(inpFile);
	free(dictWord);
	fclose(outFile);
	fclose(dictionaryFile);
	
	return (1);
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
	printf("Usage: prog5 [Text_Name] [Output_File_Name]\n");
} // end void printUsage()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-
 * parameters:		-
 * Return Value: 	-
 * Assumptions: 	-program will not attempt to use .ptrWordNotInDict of returned structure until it is initialized elsewhere
 */

t_ptrstructNovel initstructNovel()
{
	// Note: initializes all members EXCEPT .ptrWordNotInDict
	t_ptrstructNovel rtrnNovel; // pointer to structNovel structure to-be returned
	int x = 0;
	
	// allocate memory, check success
	if (!(rtrnNovel = (t_ptrstructNovel)calloc(1, sizeof(*rtrnNovel))))
	{
		printf("Fatal Error: failled call of calloc() in initstructNovel ()! terminating..\n");
		return (NULL);
	}
	// end  if (!(rtrnNovel = (t_ptrstructNovel)calloc(1, sizeof(*rtrnNovel))))
	
	// initialize each member
	rtrnNovel->totalLetters = 0;
	rtrnNovel->numWords = 0;
	
	for (x = 0; x < 26; x++) 
	{	
		rtrnNovel->letterStats[x].frequency = 0.0;
		rtrnNovel->letterStats[x].occurrences = 0;
	}
	
	rtrnNovel->numWordsOUS = 0;
	rtrnNovel->numWordsACESLMTW = 0;
	rtrnNovel->numWordsNotInDict = 0;
	rtrnNovel->ptrWordsOUS = NULL;
	rtrnNovel->ptrWordsACESLMTW = calloc(1, sizeof(t_wordACESLMTW));
	rtrnNovel->ptrWordNotInDict = calloc(1, sizeof(t_wordNotInDict));
	
	return (rtrnNovel);
} // end t_ptrstructNovel initstructNovel() 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* t_ptrstructNovel destroystructNovel(t_ptrstructNovel)
 * purpose: 		-destroy/deconstruct/unwind a structNovel structure
 * parameters:		-t_ptrstructNovel destroyStructNovel 
 * 						-pointer to structNovel structure to be destroyed
 * Return Value: 	-returns a t_ptrstructNovel NULL pointer
 * Assumptions: 	-
 */
 
 t_ptrstructNovel destroystructNovel(t_ptrstructNovel destroyStructNovel)
 {	
	int x = 0;
	
	// check that function was passed a pointer to a struct that's valid in the first place
	if (!destroyStructNovel)
		return (destroyStructNovel);
	
	// Destroy all structures (and the strings they contain) pertaining to words not in the dictionary
	if (destroyStructNovel->ptrWordNotInDict && destroyStructNovel->numWordsNotInDict)
	{
		for (x = 0; x < destroyStructNovel->numWordsNotInDict; x++)
		{
			free(destroyStructNovel->ptrWordNotInDict[x].strWord);
		}
		free(destroyStructNovel->ptrWordNotInDict);
	} 
	
	// Destroy all structures (and the strings they contain) pertaining to words containing substring OUS
	if (destroyStructNovel->ptrWordsOUS && destroyStructNovel->numWordsOUS)
	{

		for (x = 0; x < destroyStructNovel->numWordsOUS; x++)
		{
			if (destroyStructNovel->ptrWordsOUS[x].strWord)
				free(destroyStructNovel->ptrWordsOUS[x].strWord);
		}
		free(destroyStructNovel->ptrWordsOUS);
	}
	
	// Destroy all structures (and the strings they contain) pertaining to words made of characters ACESLMTW
	if (destroyStructNovel->ptrWordsACESLMTW && destroyStructNovel->numWordsACESLMTW)
	{
		for (x = 0; x < destroyStructNovel->numWordsACESLMTW; x++)
		{
			free(destroyStructNovel->ptrWordsACESLMTW[x].strWord);
		}
		free(destroyStructNovel->ptrWordsACESLMTW);
	} 
	
	// free structure, return NULL pointer to prevent dangling pointers
	free(destroyStructNovel);
	return(destroyStructNovel = NULL);
 } // end t_ptrstructNovel destroystructNovel(t_ptrstructNovel)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-fetches next word from file pointer to by ptrFile
 * parameters:		-FILE * ptrFile - file pointer
 * Return Value: 	-on success, returns ptr to string containing next word
 * 					-on failure, returns NULL
 * Assumptions: 	-returned string will be free'd or realloc'ed elsewhere
 */
 
 char * getWord(FILE * ptrFile)
{
	char * strWord = NULL, // stores next word based on fscanf call (possibly w/ hyphen)
		 * preStr = NULL;  // stores copy of strWord before hyphen (if present)
	static char * postStr = NULL; //if a hyphen is found, this contains the portion of the string after the hyphen
									  // and returns it the next time this function is called
	static short wordFound = 0;  	  // boolean, stores whether function needs to return the word store in strNextWord
									  // or call fscanf()
	int result = 0;
									  
	if (!ptrFile)
		return(NULL);
		
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
				preStr = remPunct(preStr);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-accepts a ptr to the start of a vector of t_wordACCESLMTW structures (also works with t_wordOUS
 * 							and t_WordNotInDict types) 
 * parameters:		-t_ptrwordACESLMTW ptrWordACESLMTW - ptr to the first element of a vector of structs
 * 					-int numWordsACESLMTW - number of elements in vector
 * Return Value: 	-on success, returns pointer to same vector that it was passed, only with duplicate entries removed
 * 					-on failure, returns NULL
 * Assumptions: 	-none
 */

t_ptrwordACESLMTW remDuplicatesWords(t_ptrwordACESLMTW ptrWordACESLMTW, int numWordsACESLMTW)
{
	int index = 0,		//first level loop
		index2 = 0, 	//second level loop
		len = 0;		// used to store length of .strWord member before memset'ing it
		
	// Starting from the end of the vector of wordOUS structures, iterate backwards
	// and try to find the first match
	for (index = numWordsACESLMTW - 1; index > 0; index--)
	{
		if (!ptrWordACESLMTW[index].strWord)
			continue;
		for (index2 = 0; index2 < numWordsACESLMTW; index2++)
		{
			if (!ptrWordACESLMTW[index2].strWord)
				continue;
			
			if(!strcmp(ptrWordACESLMTW[index2].strWord, ptrWordACESLMTW[index].strWord))
			{
				// Do nothing if comparing word to itself
				if (ptrWordACESLMTW[index2].wordID != ptrWordACESLMTW[index].wordID)
				{
					// clear the memory pointed to by the later occurence of the word
					// and increment the .occurrence of the structure of the first occurrence of the word
					len = strlen(ptrWordACESLMTW[index].strWord);
					memset(ptrWordACESLMTW[index].strWord, '\0', len); 
				
					ptrWordACESLMTW[index2].occurrences++;
				}
			} 
		}
		
	} // end for (; index < numWordsACESLMTW && ptrWordACESLMTW[index]; index++)
	
	return (ptrWordACESLMTW);
	
} //end t_ptrwordACESLMTW remDuplicatesWordOUS(t_ptrWordACESLMTW ptrWordACESLMTW, int numWordsACESLMTW)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * purpose: 		-prints output to file pointed by out, according to specific format
 * parameters:		-t_ptrstructNovel ptrNovel - contains all relevant stats about a novel
 * 					-FILE * out - file to output to
 * Return Value: 	-1 on success, 0 on failure
 * Assumptions: 	-none
 */

int printOutput(t_ptrstructNovel ptrNovel, FILE * out)
{
	int x = 0, // for loop index
		y = 0; // loop index
	
	if (!out || !ptrNovel)
		return 0;

	
	// Print letter statistics
	fprintf(out, "Total Novel Letters = %6d\n", ptrNovel->totalLetters);
	for (x = 0; x < 26; x++)
		fprintf(out, "Total Letters %c = %6d = %4.2f %%\n", (x + 65), ptrNovel->letterStats[x].occurrences, 100.0 * (float)ptrNovel->letterStats[x].occurrences 
													/ (float)ptrNovel->totalLetters);
	
	// Print total Novel words and word length statistics
	fprintf(out, "\nTotal Novel Words = %d\n", ptrNovel->numWords);
	for (x = 1; x <= ptrNovel->lenLngstWrd; x++)
		fprintf(out, "Total Words of Length %2d = %4d\n", x, ptrNovel->numWordsOfAllLen[x]);
		
	
	//print WordNotInDict statistics
	fprintf(out, "\nAddendum words:\n");	
		
	//iterate through all WordNotInDict structures
	for (x = 0, y = 0; x < ptrNovel->numWordsNotInDict; x++)
	{
		if (ptrNovel->ptrWordNotInDict[x].strWord)
		{
			if (ptrNovel->ptrWordNotInDict[x].strWord[0] != 0)
			{
				fprintf(out, "Word[%d] = %s [%d]\n", y, 
								ptrNovel->ptrWordNotInDict[x].strWord, ptrNovel->ptrWordNotInDict[x].occurrences);
				y++;
			}
		}
	}
	
	//print wordOUS statistics
	fprintf(out, "\nWords containing the string \"OUS\"\n");	
		
	//iterate through all wordOUS structures
	for (x = 0, y = 0; x < ptrNovel->numWordsOUS; x++)
	{
		if (ptrNovel->ptrWordsOUS[x].strWord)
		{
			if (ptrNovel->ptrWordsOUS[x].strWord[0] != 0)
			{
				fprintf(out, "SubStringWord[%d] = %s\n", y, ptrNovel->ptrWordsOUS[x].strWord);
				y++;
			}
		}
	}
	
	//print wordACESLMTW statistics
	fprintf(out, "\nWords only made up of the letters \"ACESLMTW\"\n");	
	
	//iterate through all ptrWordsACESLMTW structures
	for (x = 0, y = 0; x < ptrNovel->numWordsACESLMTW; x++)
	{
		if (ptrNovel->ptrWordsACESLMTW[x].strWord)
		{
			if (ptrNovel->ptrWordsACESLMTW[x].strWord[0] != 0)
			{
				fprintf(out, "SpecialWord[%d] = %s\n", y, ptrNovel->ptrWordsACESLMTW[x].strWord);
				y++;
			}
		}
	}
	
	return (1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
