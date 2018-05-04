/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* fileio.h                  */
#ifndef FILEIO_H_
#define FILEIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "hash.h"
#include "novel.h"
#include "list.h"

void printUsage(); // prints usage if this program is called incorrectly from commandlne
int printOutput( FILE * stream, t_structNovel * novel, struct hash_table * hash); // calculates and prints the final stats
FILE * openBook(char * filename); // when passed a path to a text file, opens a file stream to it
int getargs(int argc, char **argv, t_structNovel * novel); // reads in command line arguements, and if the expected type are found, opens text files

#endif // FILEIO_H_


