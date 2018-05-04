/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 4
*/

/* novel.c                  */

#include <stdlib.h>
#include <stdio.h>

#include "novel.h"

//initstructNovel() - initializes a novel struct containing info about an entire novel
t_structNovel * initstructNovel()
{
	t_structNovel * newNovel = calloc(1, sizeof(*newNovel));
	if (!newNovel)
		return NULL;	

	return newNovel;		
} // end t_ptrstructNovel initstructNovel() 

// destroystructNovel() - destroy/deconstruct/unwind a structNovel structure
 t_structNovel * destroystructNovel(t_structNovel * destroyStructNovel)
 {	
	if (!destroyStructNovel)
		return NULL;

	//todo: add frees of any memory allocated for members of destroyStructNovel
	if (destroyStructNovel->book[0])
		fclose(destroyStructNovel->book[0]);

	if (destroyStructNovel->book[1])
		fclose(destroyStructNovel->book[1]);

	free (destroyStructNovel);
	destroyStructNovel = NULL;

	return NULL;
 } // end t_ptrstructNovel destroystructNovel(t_ptrstructNovel)
