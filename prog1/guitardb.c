/*
Adam Van Scyoc
Sec 001
Fall 2017
Assignment 1
*/

#include <stdlib.h>
#include <stdio.h>

#include "guitardb.h"


/* struct guitardb * guitardb_create()
-Allocates and initializes a new empty guitar database
-takes no arguements
-returns instantiated, initialized guitardb struct*/
struct guitardb * guitardb_create()
{
	struct guitardb * rtrnGuitardb = malloc(sizeof(struct guitardb));
	if (!rtrnGuitardb)
		return NULL;

	// initialize all members
	rtrnGuitardb->cursor = -1; // initialized to reset/unset state
	rtrnGuitardb->numguitars = 0;

	for (int x = 0; x < MAX_GUITARS; x++)
		rtrnGuitardb->guitars[x] = NULL;

	return rtrnGuitardb;
}

/* int guitardb_add(struct guitardb * curGuitardb, struct guitardb_guitar * newGuitar)
-Add's a user-supplied guitardb_guitar struct to the database
-takes a pointer to current db. and pointer to struct to be added
-returns -1 on fail, 1 on success */
int guitardb_add(struct guitardb * curGuitardb, struct guitardb_guitar * newGuitar)
{
	// Check that guitardb exists, the guitar we are trying to add exists, and
	// add a guitar will not overflow guitardb
	if (!curGuitardb || !newGuitar || 
		curGuitardb->numguitars + 1 > MAX_GUITARS)
		return -1;
	
	// Only add if guitar is unique
	if (!guitardb_lookup(curGuitardb, newGuitar->item_key))
	{
		struct guitardb_guitar * lastGuitar = NULL;	

		// if the list is empty - ie guitardb_first returns NULL
		if (!(lastGuitar = guitardb_first(curGuitardb)))
		{
			// set first element of guitardb to newGuitar
			curGuitardb->guitars[0] = newGuitar;

			curGuitardb->numguitars = 1;
			curGuitardb->cursor = -1; // reset cursor
			return 1;
		}

		// otherwise, keep iterating through the list until the end 

		// Set the first NULL element that we found in guitardb to
		// newGuitar
		curGuitardb->guitars[curGuitardb->numguitars] = newGuitar;
		if (!curGuitardb->guitars)
			return -1;

		curGuitardb->numguitars++;
		curGuitardb->cursor = -1; // reset cursor
		return 1;
	}

	// Attempted to add a guitar that is already in the list; return error
	else
		return -1;
}

// Iterates through the database, searching for a matching item_key field
struct guitardb_guitar * guitardb_lookup(struct guitardb * curGuitardb, int item_key)
{
	// Check for case where curGuitar is NULL
	if (!curGuitardb)
		return NULL;

	struct guitardb_guitar * testGuitar = NULL;

	if (!(testGuitar = guitardb_first(curGuitardb)))
	{
		// curGuitardb is empty
		curGuitardb->cursor = -1;

		return NULL;
	}
	if (testGuitar->item_key == item_key)
	{
		// first element in curGuitardb is a match
		return testGuitar;
	}
	else
	{
		// first element is not a match; iterate through guitars
		// and look for match
		while ((testGuitar = guitardb_next(curGuitardb)))
			if (testGuitar->item_key == item_key)
				return testGuitar;

	}

	// if we didn't find it in the previous while loop, that guitar is not in db
	curGuitardb->cursor = -1; // cursor now pointing 1 beyond end of list and
		// needs to be reset

	return NULL;
}

// Searchs for an element with specified item_key, then removes it from list (but does not free it)
int guitardb_delete(struct guitardb * curGuitardb, int item_key)
{
	// check for case where curGuitardb is null or empty
	if (!curGuitardb || curGuitardb->guitars[0] == NULL)
		return -1;

	// attempt to find the guitardb_guitar element that matches item_key
	struct guitardb_guitar * delGuitar = guitardb_lookup(curGuitardb, item_key);
	if (!delGuitar)
		// guitar we've been asked to delete was not found
		return -2; // return error

	// iterate through the list, shifting ranks left
	do
	{
		// copy n+1'th element into nth element
		curGuitardb->guitars[curGuitardb->cursor] 
			= curGuitardb->guitars[curGuitardb->cursor + 1];

		curGuitardb->cursor++;	
	} while (curGuitardb->guitars[curGuitardb->cursor] != NULL);

	// Adjust guitardb's number-of-guitars tracker
	curGuitardb->numguitars--;
	
	curGuitardb->cursor = -1;
	
	return 1;
}



// Returns first guitar in database
struct guitardb_guitar * guitardb_first(struct guitardb * curGuitardb)
{
	// if curGuitardb is NULL or contains no non-NULL elements
	if (!curGuitardb || !curGuitardb->guitars[0])
		return NULL;

	curGuitardb->cursor = 0;
	return curGuitardb->guitars[0];
}


// Depending on value guitardb.cursor (which must be set to a valid value by guitardb_first or
// guitardb_lookup), fetches next (sequential) guitar in db
struct guitardb_guitar * guitardb_next(struct guitardb * curGuitardb)
{
	// if curGuitardb is NULL, empty, cursor is in reset state, or there is no next element
	if (!curGuitardb || curGuitardb->cursor == -1 || !(curGuitardb->guitars[curGuitardb->cursor + 1]) )
	{
		// reset cursor and return error
		curGuitardb->cursor = -1;
		return NULL;
	}

	return curGuitardb->guitars[++curGuitardb->cursor];	
}

// frees the guitardb (but not the guitars in the db themselves)
int guitardb_destroy(struct guitardb * curGuitardb)
{
	// check that curGuitardb is non-NULL
	if (!curGuitardb)
		return -1;
	
	free(curGuitardb);
	curGuitardb = NULL;


	return 1; // return success
}

