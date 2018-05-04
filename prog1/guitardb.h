/*
Adam Van Scyoc
Sec 001
Fall 2017
Assignment 1
*/

#ifndef GUITARDB_H_
#define GUITARDB_H_

#define MAX_GUITARS 99

/* All the attributes of a single guitar; all but item_key are user-generated
.item_key is a unique identifier (integer) that is supplied programmatically by assign1.c
note: body_type is not null-terminated and thus can't be printed as is */

struct guitardb_guitar
{
	int item_key;	/* unique id number for database */
	char make[15];	/* Fender, Gibson, Gretsch, etc */
	char model[15];	/* Telecaster, Les Paul, etx. */
	char submodel[15];	/* Deluxe, Standard, CUstom, etc. */
	char body_type[2];	/* SB solid body, HB hollow body, SH 
		semihollow */
	int year;	/* year of manufacture */
	char finish[25];	/* description of finish including
		color, binding, etc. */
	int strings;	/* Number of strings */
	int pickups; /* num of pickups */
	char neck;	/* pickup type - H humbucker, C single coil, */
		/* P P-90, F Filtertron, N none */
	char middle; /* Same as above */
	char bridge; /* Same as above */
};

/* Contains information pertaining to the database itself;
-cursor is a persisting variable that tracks the position in the list that a function working at
.numguitars is the total number of elements in the db */

struct guitardb
{
	int numguitars;	/* Number of guitars contained in db at a
		given time */
	int cursor;	/* Position relative to the list */
	struct guitardb_guitar * guitars[MAX_GUITARS];
};

// Allocates and initializes a new empty guitar database
struct guitardb * guitardb_create();

// Add's a user-supplied guitardb_guitar struct to the database
int guitardb_add(struct guitardb *, struct guitardb_guitar *);

// Iterates through the database, searching for a matching item_key field
struct guitardb_guitar * guitardb_lookup(struct guitardb *, int);

// Searchs for an element with specified item_key, then removes it from list (but does not free it)
int guitardb_delete(struct guitardb *, int);

// Returns first guitar in database
struct guitardb_guitar * guitardb_first(struct guitardb *);

// Depending on value guitardb.cursor (which must be set to a valid value by guitardb_first or
// guitardb_lookup), fetches next (sequential) guitar in db
struct guitardb_guitar * guitardb_next(struct guitardb *);

// frees the guitardb (but not the guitars in the db themselves)
int guitardb_destroy(struct guitardb *);
