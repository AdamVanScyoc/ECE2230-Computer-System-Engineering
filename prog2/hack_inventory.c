/* hack_inventory.c       */

/**************************/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 2
*/

#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "hack_object.h"
#include "hack_inventory.h"

// function declerations
h_object *h_inventory_restore_curr(h_inventory inventory, h_object * prevCurr);

//structutre that contains all the info relevant to an inventory
struct h_inventory 
{
	list invList;
};


/** Creates a new empty inventory.
 *
 * The current object is initially NULL.
 *
 * Returns the new inventory or NULL on error.
*/
h_inventory h_inventory_create(void)
{
	h_inventory newInventory = malloc(sizeof(struct h_inventory));
	if (!newInventory)
		return NULL;
	
	newInventory->invList = list_init(); 
	if (!newInventory->invList)
	{
		free(newInventory);
		return NULL;
	}

	return newInventory;
}

/** Adds a new object to the inventory.
 *
 * OBJECT is inserted into the inventory in order sorted by
 * OBJECT->class. The new object is appended to the end of all the
 * objects with the same class (see example).
 *
 * OBJECT->id must be unique within the inventory. Attempting to add
 * an object with a duplicate id is an error.
 *
 * The current object remains the same.
 *
 * Example:
 * | call                   | inventory                 |
 * |------------------------+---------------------------|
 * | inventory_add(potion1) | potion1                   |
 * | inventory_add(tool)    | tool,potion1              |
 * | inventory_add(food)    | tool,potion1,food         |
 * | inventory_add(potion2) | tool,potion1,potion2,food |
 *
 * Returns OBJECT or NULL on error.
 */
h_object *h_inventory_add(h_inventory inventory, h_object *object)
{
	// for returning the current object to where it was before this function is called
	h_object * rtrnCurr = (h_object *) list_curr(inventory->invList);

	if (!inventory || !object)
		return NULL;

	// check that an object with object's id doesn't already exist in list
	if (h_inventory_find_id(inventory, object->id))
	{
		
		// restore curr to the object it pointed to before call to this function
		h_inventory_restore_curr(inventory, rtrnCurr);
		return NULL;
	}

	// find objects of the desired class in the list
	h_object * data = (h_object *) list_first(inventory->invList);
	if (!data)
	{
		// list is empty
		if (!list_insert(inventory->invList, (list_data) object))
		{
			// if list was empty and now contains 1 object, this should
			// reset cursor to its previous value (NULL)
			list_next(inventory->invList);

			return NULL;
		}

		else
		{
			// if list was empty and now contains 1 object, this should
			// reset cursor to its previous value (NULL)
			list_next(inventory->invList);

			return object;
		}
	}

	while  (1)
	// while nodes still remain in list
	{
		if (data->class == object->class)
		{	// Found a match for class

			// loop until we find the first object whose class does 
			// NOT match object's
			while ( data->class == object->class ) 
			{
				data = (h_object *)list_next(inventory->invList);
				if (!data)
				{	// if this happens, desired class was the last class
					// in the list, therefore at this point current == NULL
					// which won't work for list_insert_before(); instead,
					// append to end of list
					if (!list_append(inventory->invList, (list_data) object))
					{	// failed to append

						// restore curr to the object it pointed to before call to this function
						h_inventory_restore_curr(inventory, rtrnCurr);

						return NULL;
					}

					// successfully appended

					// restore curr to the object it pointed to before call to this function
					h_inventory_restore_curr(inventory, rtrnCurr);

					return object;
				} // end if (!data)

			} // end while ( data->class == object->class ) 

			// now, current positioned where we want for a call to 
			// list_insert_before()
			if (!list_insert_before(inventory->invList, (list_data) object))
			{	// failed to insert (that's what she said)

				// restore curr to the object it pointed to before call to this function
				h_inventory_restore_curr(inventory, rtrnCurr);

				return NULL;
			} // end if (!list_insert_before(inventory->invList, (list_data) object))
			
			else 
				break;

		} // end if (data->class == object->class) 

		else if (data->class > object->class)
		{	// first object with a class greater that object's class;
			// desired class not found; this would be a good place to
			// insert_before
		
			// now, current positioned where we want for a call to
			// list_insert_before()
			if (!list_insert_before(inventory->invList, 
				(list_data) object))
			{	// failed to insert
				
				// restore curr to the object it pointed to before call to this function
				h_inventory_restore_curr(inventory, rtrnCurr);

				return NULL;
			}
			
			else
				break;
		} // end else if (data->class >  object->class)
	
		// iterate to next obj
		data = (h_object *) list_next(inventory->invList);
		if (!data)
		{

			// if made it to this point, an object with desired class (or one with a class
			// that is greater than the one we want) was not found in list;
			// append to end of list
			if (!list_append(inventory->invList, (list_data) object))
			{ // failled to append
				
				// restore curr to the object it pointed to before call to this function
				h_inventory_restore_curr(inventory, rtrnCurr);

				return NULL;
			}		
			
			else
				break;

		} // end if (!data)

	} // end while  (1)

	// restore curr to the object it pointed to before call to this function
	h_inventory_restore_curr(inventory, rtrnCurr);

	return object;
}

/** Removes the current object from the inventory.
 *
 * The new current object becomes the object after the current object
 * (or NULL if there is no object after the current object).
 *
 * Returns the object that was removed or NULL if the current object
 * was NULL.
 */
h_object *h_inventory_delete(h_inventory inventory)
{
	if (!inventory)
		return NULL;

	return list_remove(inventory->invList);
	
}

/** Move to the first object in the inventory.
 *
 * Sets the current object to the first object in the inventory (or
 * NULL if the inventory is empty).
 *
 * Returns the first object in the inventory or NULL if the inventory
 * is empty.
 */
h_object *h_inventory_first(h_inventory inventory)
{
	if (!inventory)
		return NULL;

	return list_first(inventory->invList);
}

/** Move to the next object in the inventory.
 *
 * Sets the new current object to the object after the current
 * object. There is no object after the NULL current object.
 *
 * Returns the object after the current object or NULL if there is no
 * object after the current object.
 */
h_object *h_inventory_next(h_inventory inventory)
{
	if (!inventory)
		return NULL;

	return list_next(inventory->invList);
}

/** Finds an object by its id.
 *
 * Sets the current object to the object whose id is equal to ID (or
 * NULL if no such object was found).
 *
 * Returns the matching object or NULL if no such object was found.
 */
h_object *h_inventory_find_id(h_inventory inventory, int id)
{
	if (!inventory)
		return NULL;
	
	// attempt to check first item in list for match
	h_object * object = (h_object *) list_first(inventory->invList);
	if (!object) // inventory->invList is empty
		return NULL;

	else if (object->id == id)
		return object;

	// iterate through rest of list, searching for a match
	while (1)
	{
		object = (h_object *) list_next(inventory->invList);
		if (!object) // reached end of list without match
			return NULL;

		else if (object->id == id)
			return object;
	}
}

/** Destroys the inventory and frees its memory.
 *
 * Storage for any objects in the inventory is never freed.
 */
void h_inventory_destroy(h_inventory inventory)
{
	if (!inventory)
		return;

	list_finalize(inventory->invList);
	free(inventory);
	inventory = NULL;	
}

/** Restores the current item to the object pointed to by prevCurr
*
* Used in h_inventory_add() to satisfy requirement that 
* "the current item is not changed"
*
*/
h_object *h_inventory_restore_curr(h_inventory inventory, h_object * prevCurr)
{
	if (!inventory)
		return NULL;

	// if the prevCurr was NULL, iterate to a NULL item in the inventory
	// as way of circumventing the fact that we can't directly manipulate
	// inventory->invList->current
	if (!prevCurr)
	{
		while (1)
		{	// find a NULL object in list

			if (!list_next(inventory->invList))
				// previous current item was NULL, so return NULL
				return NULL;
		}
	}
	
	else
	{	// find the object that prevCurr points to, and set current equal to it
		h_inventory_find_id(inventory, prevCurr->id);
	
		return prevCurr;
	}
		
}
	


/**************************/
