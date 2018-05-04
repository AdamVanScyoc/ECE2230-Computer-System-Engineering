Adam Van Scyoc (Avanscy@clemson.edu)
ECE 2230 SEC 001
FALL 2017
ASSIGNMENT 4

Readme.txt

The project files:

list.h/.c:
	Contains the genereric implementation of a 2-way linked list that was originally created for program 2

fileio.h/.c:
	Contains functions that perform file input and output operations. Here "file" includes outputting to stdout 
		(with fprintf and printf). 	
	printUsage() - standard command-line program function to display how the program should be invoked
	openBook() - opens file pointers for I/O to text files that were supplied from the command line
	getargs() - main() calls this function and passes it argc and argv. getargs uses getopt to process command line options,
		 hopefully find 2 valid paths to text files; it then calls openBook on those paths and saves file pointers to them

novel.h/.c:
	defines the structWord structure that stores information about an individual word,
	 the structNovel structure that contains all the informations/statistics about both books as
	well as their file pointers, and an initialization/constructor and de-initizalition/destructor for structNovel structures

hash.h/.c: 
	define the hash_table structure that encapsulates all the information about an entire 
	hash table, as well as all the operations there-on:
		-hash_init() - constructor
		-hash_destroy() - destructor
		-hash_insert() - calls the hash function and inserts into linked list found at that 
		address in hash table
		-hash_search() iterates through entire hash_table - and all the linked lists - 
		searching for a particular key

How to Compile:
	Simply run make with the included Makefile.
	

Calling the program from the command-line 
	
	The program looks for 2 arguements: '-1' and '-2'. the option '-1' should be followed by
	the path to the first of the 2 text files; option '-2' should be followed by the second.
	With the files 'drac.txt' and 'frank.txt' (as long as they are spelled exactly this way
	with the same capitalization), order does not matter.

	Ex;

	./prog4 -1 drac.txt -2 frank.txt

	... or equivalently

	./prog4 -1 frank.txt -2 drac.txt


