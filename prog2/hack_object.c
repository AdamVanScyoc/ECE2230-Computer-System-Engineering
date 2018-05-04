/**************************/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 2
*/

/* hack_object.c          */

#include <stdio.h>

#include "hack_object.h"

/** Print the string representation of an object.
 *
 * Writes the string representation of OBJECT to STREAM.
 *
 * Returns the number of characters printed (not including the
 * trailing '\0') or -1 on error.
 */
int h_object_print(FILE *stream, const h_object *object)
{
	if (!stream || !object)
		return -1;	

	int charsPrinted = 0;
	charsPrinted += fprintf(stream, "\nObject ID: %d\n", object->id);
	charsPrinted += fprintf(stream, "Item Location - Level: %d\tX Coord: %d\tY Coord:%d\n", 
		object->loc.l, object->loc.x, object->loc.y);
	charsPrinted += fprintf(stream, "Object Type: %d\n", object->type);
	charsPrinted += fprintf(stream, "Object Class: %d\n", object->class);
	charsPrinted += fprintf(stream, "Object Status: %d\n", object->status);
	charsPrinted += fprintf(stream, "Object Charge: %d\n", object->charge);
	charsPrinted += fprintf(stream, "Object Weight: %d\n", object->weight);
	
	return charsPrinted;	

}

/** Compare two game objects.
 *
 * Returns an integer greater than, equal to, or less than 0,
 * according as the h_object LEFT is greater than, equal to, or less
 * than the h_object RIGHT.
 */
int h_object_compare(const void * left, const void * right)
{
	h_object * templ = (h_object *)left;
	h_object * tempr = (h_object *)right;

	if (templ->id > tempr->id)
		return 1;
	else if (templ->id < tempr->id)
		return -1;
	

	if (templ->loc.l > tempr->loc.l)
		return 1;
	else if (templ->loc.l < tempr->loc.l)
		return -1;
	
	if (templ->loc.x > tempr->loc.x)
		return 1;
	else if (templ->loc.x < tempr->loc.x)
		return -1;
	
	if (templ->loc.y > tempr->loc.y)
		return 1;
	else if (templ->loc.y < tempr->loc.y)
		return -1;
	
	if (templ->type > tempr->type)
		return 1;
	else if (templ->type < tempr->type)
		return -1;
	
	if (templ->class > tempr->class)
		return 1;
	else if (templ->class < tempr->class)
		return -1;
	
	if (templ->status > tempr->status)
		return 1;
	else if (templ->status < tempr->status)
		return -1;
	
	if (templ->charge > tempr->charge)
		return 1;
	else if (templ->charge < tempr->charge)
		return -1;
	
	if (templ->weight > tempr->weight)
		return 1;
	else if (templ->weight < tempr->weight)
		return -1;
	
	return 0;
}
/**************************/
