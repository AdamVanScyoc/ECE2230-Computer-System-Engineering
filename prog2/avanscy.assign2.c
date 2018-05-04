/*******************************/
/* avanscy.assign2.c */
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "hack_object.h"
#include "hack_inventory.h"

int main()
{
	char * lineptr = NULL, 
	     command[15]; 
	size_t n = 0;
	int numitems = 0; // tracks number of items stored in inventory
	int inpItemKey = 0;

	h_object ** newObject = NULL;

	// create inventory
	h_inventory newInv = h_inventory_create();
	if (!newInv)
	{
		printf("Fatal error creating inventory! terminating...\n");
		return -1;
	}

	while (1) {
		// print menu
		printf("\nPlease choose from the following commands:\nADD\nLOOK item-key\nDEL item-key\nLIST\nFIRST\nNEXT\nPREV\nLAST\nQUIT\n");
		
		// read in user response
		getline(&lineptr, &n, stdin);
		if (strstr(lineptr, "\n"))
			*(strstr(lineptr, "\n")) = '\0'; // change trailing \n byte to a NULL byte

		// determine if we need to look for an integer input
		if (sscanf(lineptr, "%s %d", command, &inpItemKey) == 1)
		{
			// ADD, LIST, or QUIT was selected; disrefard integer arguement
			if (!strcmp(command, "ADD"))
			{		
				// prompt for each field and populate corresponding g
					//h_object structure's field

				newObject = realloc(newObject, (numitems+1)*sizeof(h_object *));
				newObject[numitems] = malloc(sizeof(h_object));

				if (!newObject[numitems])
				{
					printf("Error: malloc() failed in ADD handler. Continuing from menu.\n");
					continue;
				}
				
				// use loops to ensure a valid input is received for each field

				// Get type
				while (1) {
					printf("\nPlease input an integer for the Objects type:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", (int *)&newObject[numitems]->type))

					break;
				} // end input for h_object.type

				// get class
				while (1) {
					printf("Please input an integer for the Objects class:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", (int *)&newObject[numitems]->class))

					break;
				} // end input for h_object.class

				// get status
				while (1) {
					printf("Please input an integer for the Object's status:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", (int *)&newObject[numitems]->status))

					break;
				} // end input for h_object.status

				// get charge
				while (1) {
					printf("Please input an integer for the Object's charge:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", &newObject[numitems]->charge))

					break;
				} // end input for h_object.charge

				// get weight
				while (1) {
					printf("Please input an integer for the Object's weight:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", &newObject[numitems]->weight))

					break;
				} // end input for h_object.weight

				// get loc
				while (1) {
					printf("Please input the Object's location:\n");
					printf("\tInput the level: ");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (!sscanf(lineptr, "%d", &newObject[numitems]->loc.l))
						continue;

					printf("\n\tInput the x coordinate: ");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (!sscanf(lineptr, "%d", &newObject[numitems]->loc.x))
						continue;

					
					printf("\n\tInput the y coordinate: ");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (!sscanf(lineptr, "%d", &newObject[numitems]->loc.y))
						continue;

					else	// all fields we scanned in properly
					{
						printf("\n");
						break;
					}
				} // end input for h_object.loc

				newObject[numitems]->id = numitems + 1;

				if (!h_inventory_add(newInv, newObject[numitems]))
				{
					printf("\nError: failed to add new object to inventory!\n");
					continue;
				}
				++numitems;
				
			} // end if (!srcmp(command, "ADD"))

			else if (!strcmp(command, "LIST"))
			{
				// fetch and list first object
				h_object * listObj = h_inventory_first(newInv);
				if (!listObj)
				{
					// an error occured - inventory likely empty
					printf("Error attempting to list inventory - inventory is empty.\n\n");
					continue;
				}
				h_object_print(stdout, listObj);

				// iterate through rest of list - list all remaining guitars
				while ((listObj = h_inventory_next(newInv)))
				{
					h_object_print(stdout, listObj);
				} // end while ((listGuitar = guitardb_next(guitardb))

			} // end else if (!strcmp(command, "LIST"))

			else if (!strcmp(command, "FIRST"))
			{
				printf("Retrieving first object in inventory...\n");

				h_object * firstObj = h_inventory_first(newInv);
				if (!firstObj)
				{
					printf("Error finding first object! \n");
					continue;
				}

				h_object_print(stdout, firstObj);
				
			} // end else if (!strcmp(command, "FIRST"))
			
			else if (!strcmp(command, "NEXT"))
			{
				printf("Retrieving next object in inventory...\n");

				h_object * nextObj = h_inventory_next(newInv);
				if (!nextObj)
				{
					printf("Error finding next object! \n");
					continue;
				}

				h_object_print(stdout, nextObj);
				
			} // end else if (!strcmp(command, "NEXT"))

			else if (!strcmp(command, "LAST"))
			{
				printf("Retrieving last object in inventory...\n");

				// Use a "follower" to find the first NULL object in inventory
				// and print the object that preceeds it

				h_object * lastObj2 = NULL;
				h_object * lastObj1 = h_inventory_first(newInv);
				if (!lastObj1)
				{	// inventory is empty
					printf("Error finding last object! \n");
					continue;
				}

				// loop until we find first NULL object
				// print it's predecessor
				while (1)
				{
					lastObj2 = h_inventory_next(newInv);
					if (!lastObj2)
					{
						h_object_print(stdout, lastObj1);
						break;
					}
					
					lastObj1 = lastObj2;
				}	
				
			} // end else if (!strcmp(command, "LAST"))

			else if (!strcmp(command, "PREV"))
			{
				printf("Retrieving previous object in inventory...\n");

				// use a 2 followers; print object that is 2 objects before 
				// the node that is returned by h_inventory_next()
				// I acknowledge that this is a bad hack, but I can't think 
				// of any other way to achieve this
				h_object * prevObj0 = NULL;
				h_object * prevObj1 = NULL;
				h_object * prevObj2 = NULL;
				h_object * currObj = h_inventory_next(newInv);

				//now iterate from start of inventory w/ 2 followers
				prevObj0 = h_inventory_first(newInv);
				prevObj1 = h_inventory_next(newInv);
				prevObj2 = h_inventory_next(newInv);

				// currObj cannot be the first in the inventory,
				// check prevObj1 as possible match
				if (prevObj1 == currObj)
				{
					// Previous element does not exist
					printf("The is no previous element - current element is head!\n");
					continue;
				}

				// iterate through rest of list to locate positon of currOBj
				while (1)
				{
						
					if (prevObj2 == currObj)
					{	// previous element is prevObj0; print it
						h_object_print(stdout, prevObj0);
						break;
					}
					// increment prevObj2 and its 2 followers by 1 as we iterate through inventory
					prevObj0 = prevObj1;
					prevObj1 = prevObj2;
					prevObj2 = h_inventory_next(newInv);
					if (!prevObj2)
						//reached end of list
						break;
				}

			} // end else if (!strcmp(command, "PREV"))

			else if (!strcmp(command, "QUIT"))
				break;

		} // end if (sscanf(lineptr, "%s %d", command, &inpItemKey));

		else if (sscanf(lineptr, "%s %d", command, &inpItemKey) == 2)
		{ // if two arguments - a string and an int - were matched from menu input
			// either LOOK or DEL was selected

			if (!strcmp(command, "LOOK"))
			{
				printf("Looking for object %d ...\n", inpItemKey);

				h_object * lookObj = h_inventory_find_id(newInv, inpItemKey);
				
				if (!lookObj)
				{
					printf("Data not found.\n");
					continue;
				}
	
				printf("Looking up object %d ...\n", inpItemKey);
				h_object_print(stdout, lookObj);
			} // end if (!strcmp(command, "LOOK"))

			if (!strcmp(command, "DEL"))
			{
				printf("Deleting object %d ...\n", inpItemKey);

				// position current at item to be delete, if it exists
				if (!h_inventory_find_id(newInv, inpItemKey))
				{
					printf("Data not found.\n");
					continue;
				}

				// attempt to delete object
				h_object * delObj = h_inventory_delete(newInv);
				if (!delObj)
				{
					printf("Data not found.\n");
					continue;
				}

				printf("Data deleted.\n");		
			} // end 

		} // end else if (sscanf(lineptr, "%s %d", command, &inpItemKey) == 2)


	} // end while(1)
	
	// free objects
	for (int y = 0; y < numitems; y++)
		free(newObject[y]);

	free(newObject);

	// destroy inventory
	h_inventory_destroy(newInv);

	free (lineptr);

	return 0;
}

