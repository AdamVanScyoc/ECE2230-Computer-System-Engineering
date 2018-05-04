/*-------------------------*/
/* list.c                  */

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

// Structure that describes a single node of the two-way linked list
typedef struct listnode
{
	list_data data;
	struct listnode * right;
	struct listnode * left;
} listnode_t, * ptrlistnode;

// Structure that describes an entire two-way linked list
typedef struct list
{
	ptrlistnode head;
	ptrlistnode tail;
	ptrlistnode current;	
} list_t;

/** Allocates and initializes a new empty list.
 *
 * Returns a new empty list or NULL if there was an error.
 */
list list_init(void)
{
	list newList = malloc(sizeof(struct list));
	if (!newList)
		return NULL;

	newList->head = NULL;
	newList->tail = NULL;
	newList->current = NULL;	
	
	return (newList);	
}

/* Destroys the list and frees its memory.
 *
 * Frees all memory allocated for the list. List data is not freed.
 */
void list_finalize(list lst)
{
	// use recursion; split up head and tail while there are more than one nodes in list
	// and free head
	ptrlistnode temp;	// used to store address of 2nd node as we free 1st

	if (!lst)	// make sure lst is valid
		return; 

	else if (!lst->head) // make sure lst is not empty
	{
		free(lst);
		lst = NULL; // prevent dangling pointer
		return;
	}

	if (!lst->head->right) // if only 1 node (lst->head) remains
	{
		//base case; free lst->head, set everything to NULL
		free(lst->head);
		lst->head = NULL;
		lst->tail = NULL;
		lst->current = NULL;	// prevents dangling pointers
	
		free(lst);	// finally. free lst itself and return
		lst = NULL;
		return;
	}

	else 
	{
		// free first element of list
		temp  = lst->head;
		lst->head = lst->head->right;
		free(temp);

		list_finalize(lst);
	}
}

/** Inserts at the head of the list.
 *
 * DATA becomes the new head of the list and DATA's successor is the
 * previous head of the list. DATA must not be NULL.
 *
 * The current item is set to DATA.
 *
 * On error the list is not modified.
 *
 * Returns DATA or NULL on error.
 */
list_data list_insert(list lst, list_data data)
{
	if (!lst || !data)
		return NULL;

	//lst->head and lst->tail must both be either defined or undefined
	if ((!lst->head && lst->tail) || (lst->head && !lst->tail))
		return NULL;

	// Allocate temporary listnode that we will prepend onto list
	ptrlistnode temp = malloc(sizeof (struct listnode));
	if (!temp)
		return NULL;

	// Set up temp to be prepended to list
	temp->data = data;
	temp->left = NULL; // since temp will become the very first element, 
				// temp.left should point to NULL

	// case where lst contains no nodes prior to list_insert() being called
	if (!lst->head)
	{
		temp->right = NULL;
		lst->head = temp;
		lst->tail = temp;
	}

	// case where lst contains 1 or more nodes propr to list_insert being called
	else
	{
		temp->right = lst->head;
		lst->head->left = temp;
		lst->head = temp;
	}

	// set current item
	lst->current = lst->head;

	return data;
}

/** Appends to the tail of the list.
 *
 * DATA becomes the new tail and DATA's predecessor is the previous
 * tail of the list. DATA must not be NULL.
 *
 * The current item is set to DATA.
 *
 * On error the list is not modified.
 *
 * Returns DATA or NULL on error.
 */
list_data list_append(list lst, list_data data)
{
	if (!lst || !data)
		return NULL;

	// check that both head and tail exist or are both NULL
	if ((!lst->head && lst->tail) || (lst->head && !lst->tail))
		return NULL;

	// Allocate temporary listnode that we will append onto list
	ptrlistnode temp = malloc(sizeof (struct listnode));
	if (!temp)
		return NULL;

	// Set up temp to be appended to list
	temp->data = data;
	temp->right = NULL; // since temp will become the very last element, 
				// temp.right should point to NULL

	// case where lst contains no nodes prior to list_append being called
	if (!lst->head)
	{
		temp->left = NULL;
		lst->head = temp;
		lst->tail = temp;
	}

	// case where lst contains 1 or more nodes prior to list_append being called
	else
	{
		temp->left = lst->tail;
		lst->tail->right = temp;
		lst->tail = temp;
	}

	// set current to new last list node
	lst->current = lst->tail;

	return data;
}

/** Inserts before the current item.
 *
 * DATA's predecessor will be the current item's predecessor. DATA's
 * successor will be the current item. DATA must not be NULL.
 *
 * The current item becomes DATA.
 *
 * It is an error if the current item is NULL. On error the list is
 * not modified.
 *
 * Returns DATA or NULL on error.
 */
list_data list_insert_before(list lst, list_data data)
{
	if (!lst || !data || !lst->current)
		return NULL;

	// head, tail, and current all need to be defined or all be NULL
	if (!lst->head && !lst->tail && !lst->current)
		// everything ok if all equal NULL
		;
	else if (lst->head && lst->tail && lst->current)
		// everything ok if all are defined
		;
	else
		// some are defined, some aren't; return error
		return NULL;

	// Allocate temporary listnode that we will insert into list
	ptrlistnode temp = malloc(sizeof (struct listnode));
	if (!temp)
		return NULL;

	// Set up temp to be appended to list
	temp->data = data;

	// case where lst contains no nodes prior to list_insert_before being called
	if (!lst->head)
	{
		temp->left = NULL;
		temp->right = NULL; // since temp will become the very last element, temp.right should point to NULL

		lst->head = temp;
		lst->tail = temp;
	}
	
	// case where lst contains 1 node or current is the first node
	else if (!lst->current->left) // if there is no node prior to current...
	{
		temp->left = NULL;
		temp->right = lst->current;
		
		lst->current->left = temp;
		
		lst->head = temp;
	}

	// case where lst contains 2 or more nodes prior to list_inesert_before() being called
	// and lst->current is not the first node
	else if (lst->head && lst->head != lst->tail && lst->current->left)
	{
		temp->left = lst->current->left;
		temp->right = lst->current; 
	
		// insert temp into list
		lst->current->left->right = temp;
		lst->current->left = temp;
	}

	// set current to newly inserted node
	lst->current = temp;

	return data;
}

/** Inserts after the current item.
 *
 * DATA's predecessor will be the current item. DATA's successor will
 * be the current item's successor.  DATA must not be NULL.
 *
 * The current item becomes DATA.
 *
 * It is an error if the current item is NULL. On error the list is
 * not modified.
 *
 * Returns DATA or NULL on error.
 */
list_data list_insert_after(list lst, list_data data)
{
	if (!lst || !data || !lst->current)
		return NULL;

	if (!lst->head || !lst->tail)
		return NULL;

	// Allocate temporary listnode that we will insert into list
	ptrlistnode temp = malloc(sizeof (struct listnode));
	if (!temp)
		return NULL;

	// Set up temp to be inserted into to list
	temp->data = data;

	// case where lst contains no nodes prior to list_insert_after being called
	if (!lst->head)
	{
		temp->right = temp->left = NULL;
		lst->head = lst->tail = temp;	
	}
	
	// case where lst contains 1 node or current is the last node
	else if (!lst->current->right) // if there is no node after to current...
	{
		temp->right = NULL;
		temp->left = lst->tail;

		lst->tail->right = temp;

		lst->tail = temp;		
	}

	// case where lst contains 2 or more nodes prior to list_inesert_after() being called
	// and lst->current is not the last node
	else if (lst->head && lst->head != lst->tail && lst->current->right)
	{
		temp->right = lst->current->right;
		temp->left = lst->current;
		
		lst->current->right->left = temp;
		lst->current->right = temp;
	}

	// set current to newly inserted node
	lst->current = temp;

	return data;
}

/** Removes the current item from the list.
 *
 * Sets the new current item to the successor of the removed current
 * item. Does nothing if the current item is NULL.
 *
 * Returns the removed item (may be NULL).
 */
list_data list_remove(list lst)
{
	ptrlistnode temp_right = NULL; // temporarily saves pointer to lst->current's successor

	if (!lst)
		return NULL;

	// head, tail, and current all need to be defined or all be NULL
	if (!lst->head && !lst->tail && !lst->current)
		// everything ok if all equal NULL
		;
	else if (lst->head && lst->tail && lst->current)
		// everything ok if all are defined
		;
	else
		// some are defined, some aren't; return error
		return NULL;

	if (!lst->current) // "Does nothing if the current item is NULL"
		return NULL;
	
	// save a pointer to lst->data to be returned later
	list_data * data = lst->current->data;
	
	// case where lst->current is only node in lst
	if (!lst->current->left && !lst->current->right)
	{
		free(lst->current);

		// prevent lst->head, lst->tail, or lst->current from becoming dangling pointers
		lst->head = lst->tail = lst->current = NULL;
	}

	// case where lst->current is first node in lst
	else if (!lst->current->left && lst->current->right)
	{
		// save pointer to successor to lst->current
		temp_right = lst->current->right;
		
		// adjust lst->current's successor to become head of lst
		lst->current->right->left = NULL;	

		free(lst->current);
		
		lst->current = lst->head = temp_right;
	}

	// case where lst->current is somwhere in the middle of the list
	else if (lst->current->left && lst->current->right) 
	{
		// prepare adjacent nodes for lst->current's removal
		lst->current->right->left = lst->current->left;
		lst->current->left->right = lst->current->right;

		// save pointer to succeeding node before freeing
		temp_right = lst->current->right;

		// remove lst->current
		free(lst->current);
	
		//set lst->current to successor
		lst->current = temp_right;
	}

	// case where lst->current is the last node in list
	else if (lst->current->left && !lst->current->right)
	{
		// reset tail to lst->current's predecessor
		lst->tail = lst->current->left;
		lst->tail->right = NULL;

		free(lst->current);
		
		lst->current = NULL;
	}

	return data;
}

/** Returns the list head.
 *
 * Sets the current item to the list head or NULL.
 *
 * Returns the list head or NULL if the list is empty.
 */
list_data list_first(list lst)
{
	if (!lst) // if lst is uninitialized, return NULL
		return NULL;

	if (!lst->head) // "returns [...] or NULL if empty" but also "sets the current item to [...] or NULL
		// therefore, set current to NULL and return NULL
	{
		lst->current = NULL;
		return NULL;
	}

	lst->current = lst->head;

	return lst->head->data;
}

/** Moves the current item forward in the list.
 *
 * Sets the new current item to the successor of the current item. If
 * the current item is the list tail or the current item is NULL then
 * the new current item is NULL.
 *
 * Returns the new current item.
 */
list_data list_next(list lst)
{
	if (!lst)
		return NULL;

	// if the current item is the list tail or the current item is NULL then new current item is NULL
	//Note: works even if lst->tail is NULL - no need to NULL-check tail
	if ( !lst->current || lst->current == lst->tail)
		return (lst->current = NULL);

	// set current to the next element in list
	// no need to NULL-check right
	lst->current = lst->current->right;

	return lst->current->data;
}


/** Moves the current item backward in the list.
 *
 * Sets the new current item to the predecessor of the current
 * item. If the current item is the list head or the current item is
 * NULL then the new current item is NULL.
 *
 * Returns the new current item.
 */
list_data list_prev(list lst)
{
	if (!lst)
		return NULL;

	if (!lst->current || lst->current == lst->head)
		return (lst->current = NULL);

	// set current to the previous node in list
	lst->current = lst->current->left;

	return lst->current->data;
}

/** Returns the list tail.
 *
 * Sets the current item to the list tail or NULL.
 *
 * Returns the list tail or NULL if the list is empty.
 */
list_data list_last(list lst)
{
	if (!lst || !lst->head) // if lst is uninitialized or empty
		return NULL;

	return (lst->current = lst->tail);
}

/** Returns the current item of the list.
 *
 * Returns the current item of LST (may return NULL).
 */
list_data list_curr(list lst)
{
	if (!lst)
		return NULL;

	return lst->current;
}
/*-------------------------*/
