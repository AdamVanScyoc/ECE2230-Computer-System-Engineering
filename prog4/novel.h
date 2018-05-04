/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* novel.h                  */
#ifndef NOVEL_H_
#define NOVEL_H_

// statistics about each text file of a novel
struct structNovel {

	long int linecount[2];
	long int wordcount[2];

	long int uniquewordcount[2];
	long int wordinbothcount; // count of words that occur in both books
	long int uniquewordinbothcount;

	char longest[2][5][100]; 		// the 5 longest words found
	long int longestcount[2][5]; 		// ... and the number of occurrences of each

	char longestinboth[5][100]; 	// longest words that occur in both books
	long int longestinbothcount[5]; 	// ... and the number of occurrences of each
	
	char mostused[2][15][100];	 	// 15 most-frequent words in each book
	long int mostusedcount[2][15];	// ... and the number of occurrences of each

	char leastused[2][5][100];		// 5 least-frequent words in each book
	long int leastusedcount[2][5];

	char mostusedinboth[15][100];	// 15 most-freq. in either book
	long int mostusedinbothcount[15];

	char leastusedinboth[5][100];	// 5 least used words across both books
	long int leastusedinbothcount[5];
	
	FILE * book[2];			// file pointer to the text files themselves
};

typedef struct structNovel t_structNovel;

struct structWord {
	char word[100]; // a statically-declared copy of the word itself to be used as a hash key
	long count[2];	// count of the word in each book
};

typedef struct structWord t_structWord;

//Structure constructors/destructors
t_structNovel * initstructNovel();
t_structNovel * destroystructNovel(t_structNovel * destroyStructNovel);

#endif // NOVEL_H_
