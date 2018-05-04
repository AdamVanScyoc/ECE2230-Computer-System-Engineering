/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* hash.c                  */

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "list.h"


// the hash function itself
int h(char *p, int m)
{
    int flip = 0;
    int len = 0;
    int s = 0;
    int shortcnt = 0;
    unsigned short *sp = NULL;

    len = strnlen(p, WSZ);
    shortcnt = len / 2;
    sp = (unsigned short *)p;
    unsigned short hval = 0x5ab6;

    /* folding the string by twos */
    for (s = 0; s < shortcnt; s++)
    {
        unsigned short newv;
        newv = *sp;
        if (flip)
        {
            newv <<= 1;
        }
        flip = !(flip);
        hval ^= newv;
        sp++;
    }
    /* if odd size string double up last char */
    if (len & 0x0001)
    {
        hval ^= ((((short)(p[len-1])) << 8) | (short)(p[len-1]));
    }
    return (int)hval % m;
}


// initializes hash table structure and its members
struct hash_table * hash_init()
{
	struct hash_table * newHash = calloc(1, sizeof(*newHash));
	if (!newHash)
		return NULL;

	// allocate an dynamic array of 8191 linked list structures; 1 for every hash table entry
	newHash->table = calloc(HSZ + 1, sizeof(struct hash_table *));
	if (!newHash->table)
	{
		// fail gracefully
		free(newHash);
		return NULL;
	}
	// each of the 8191 pointers stored in newHash->table will be NULL
	// list_init() gets called on each list once the first item is being inserted there

	newHash->size = 8191;
	
	return (newHash);
}


// inserts into hash table - calls hash function; if a linked-list exists at that address in hash table, appends onto taik
// of list; otherwise, initializes a linked list at that address THEN appends there
t_structWord * hash_insert(struct hash_table *p, char *key, struct structWord data, int booknum)
{
	t_structWord * dup = NULL;	// for finding duplicates

	if (!p || !key)
	{
		#ifdef DEBUG 
		printf("Error in hash_insert().\n");
		#endif //DEBUG
		return NULL;
	}

	struct structWord * temp  = calloc(1, sizeof(struct structWord));
	if (!temp)
	{
		#ifdef DEBUG
		printf("Error in hash_insert: calloc returned NULL!");
		#endif //DEBUG
		return NULL;
	}

	// Check for duplicates, and if found, instead of appending to the list, increment the
	// prexisting word's counter
	// NOTE: the main program also checks for duplicates; the code is also included here
	// for completion's sake and for modulatity

	dup = hash_search(p, key);
	if (dup)	// a duplicate was found
	{
		dup->count[booknum-1]++;

		free(temp);
		return dup;
	}

	int hval = h(key, HSZ);
	//*temp = data;
	temp->count[0] = data.count[0];
	temp->count[1] = data.count[1];

	// use scanf to prevent garbage from getting copied over from the stack when copying data.word
	int wordsfound = sscanf(data.word, "%s", temp->word);
	if (wordsfound == EOF || wordsfound == 0)
	{
		#ifdef DEBUG
		printf("Error in hash_insert(): failled to match any valid words!\n");
		#endif //DEBIG
		return NULL;
	}
	

	if (!p->table[hval])
	{
		p->table[hval] = list_init();
		if (!p->table[hval])
		{
			#ifdef DEBUG
			printf("Error in hash_insert(): list_init() returned NULL!\n");
			#endif //DEBUG
			return NULL;
		}
	}

	if (!list_append(p->table[hval], temp))
	{
		#ifdef DEBUG 
		printf("Error in hash_insert().\n");
		#endif //DEBUG
		// fail gracefully; undo the changeds we've made since appending failled
		list_finalize(p->table[hval]);
		return NULL;
	}

	return temp;
}

// searchs entire hash table for a given key
void *hash_search(struct hash_table *p, char *key)
{
	if (!p || !key)
		return NULL;

	int hval = h(key, HSZ);

	if (!p->table[hval])
		// no linked list at this key; key not found 
		return NULL;

	list_data temp = list_first(p->table[hval]);
	if (!temp)
		return NULL;

	do {
		if (!strcmp(key, temp->word))
		{	
			// found the key
			return temp;
		}
		temp = list_next(p->table[hval]);
	} while (temp);

	// if we got to this point, key was not found
	return NULL;
}

// de-initializes hash table struct and its members
void hash_destroy(struct hash_table * delHash)
{
	struct structWord * delWord = NULL;

	if (!delHash)
		return;

	if (delHash->table)
	{
		for (int u = 0; u < delHash->size; u++)
		{
			if (delHash->table[u])
			{
				// iterate through entire list and free each word stored therein
				delWord = list_first(delHash->table[u]);
				if (delWord)
				{
					free(delWord);
				} // end if (delWord)

				while ((delWord = list_next(delHash->table[u])))
				{
					free(delWord);
					delWord = NULL;
				};

				list_finalize(delHash->table[u]);
			}
		}
		free(delHash->table);
		delHash->table = NULL;
	}

	free(delHash);
}
/*-------------------------*/
