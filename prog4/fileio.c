/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* fileio.c */

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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printUsage()
{

	printf("Usage: prog5 -1 First_Text_Name.txt -2 Second_Text_Name.txt\n");

} // end void printUsage()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// computes and prints statistical output to file pointed by stream, according to specific format

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

/////////////////////////////////// COMPUTE STATISTICS ///////////////////////////////////////////

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
	fprintf(stream, "\n");

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// attempts to open text file at address 'filename' and returns a file pointer to that file
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
	
	// attempt to open file
	novel = fopen(filename, "r");
	if (!novel)
	{
		printf("Fatal Error: attempt to open %s failed! terminating...\n", filename);
		return NULL;
	}

	return novel;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// parses command line inputs populats the file pointers in novel.book[]

int getargs(int argc, char **argv, t_structNovel*  novel)
{
	int thisbook = 0, // used to specify the order that the books are stored 
		c = 0; // used when getting arguements

	// get filenames of text files that were provided as arguments
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
						if (novel->book[0])
						{
							fclose(novel->book[0]);
							novel->book[0] = NULL;
						}

						if (novel->book[1])
						{
							fclose(novel->book[1]);
							novel->book[1] = NULL;
						}
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
						// exit gracefully
						if (novel->book[0])
						{
							fclose(novel->book[0]);
							novel->book[0] = NULL;
						}

						if (novel->book[1])
						{
							fclose(novel->book[1]);
							novel->book[1] = NULL;
						}
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
		printf("Error: you cannot specify the same book twice!\n");
		printUsage();
		
		if (novel->book[0])
		{
			fclose(novel->book[0]);
			novel->book[0] = NULL;
		}

		if (novel->book[1])
		{
			fclose(novel->book[1]);
			novel->book[1] = NULL;
		}

		novel = destroystructNovel(novel);
		return -1;
	}

	// make sure 2 book were successfully opened
	if (!novel->book[0] || !novel->book[1])
	{
		printf("Error: Failled to open both books that were specified! terminating...!\n");
		printUsage();
		
		if (novel->book[0])
		{
			fclose(novel->book[0]);
			novel->book[0] = NULL;
		}

		if (novel->book[1])
		{
			fclose(novel->book[1]);
			novel->book[1] = NULL;
		}

		novel = destroystructNovel(novel);
		return -1;
	}

	// return success
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
