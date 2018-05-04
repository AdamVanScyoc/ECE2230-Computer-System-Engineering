/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* hash.h                  */
#ifndef HASH_H_
#define HASH_H_

#define WSZ 256
#define HSZ 8191

#include "novel.h"

struct hash_table
{
	struct list **table; /* an array of linked lists - dynamically allocated */
	int size;                   /* number of elements in the array */
};

int h(char *p, int m); // hash function
struct hash_table * hash_init(); // initializes hash table structure and its members
void hash_destroy(struct hash_table * delHash); // de-initializes hash table struct and its members
t_structWord * hash_insert(struct hash_table *p, char *key, struct structWord data, int booknum); // inserts into hash table
void *hash_search(struct hash_table *p, char *key); // searchs entire hash table for a give key

#endif //HASH_H_
