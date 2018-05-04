/*
Adam Van Scyoc
Sec 001
Fall 2017
Assignment 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "guitardb.h"

int doListGuitar(struct guitardb_guitar * listGuitar);

int main()
{
	char * lineptr = NULL, 		// used by getline to receive user input
	     command[15]; 		// used when parsing getline into multple arguments
	size_t n = 0;			// size arguement for getline
	int inpItemKey = 0,		// the user-inputted Item Key for LOOK and DEL
		numguitars = 1;		// tracks total guitars added so that we can assign a UID
	
	struct guitardb * guitardb = guitardb_create();	

	while (1) {
		// print menu
		printf("\nPlease choose from the following commands:\nADD\nLOOK item-key\nDEL item-key\nLIST\nQUIT\n");
		
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
				struct guitardb_guitar * newGuitar = malloc 
					(sizeof(struct guitardb_guitar));
				if (!newGuitar)
					return 0;
				// prompt for each field and populate corresponding g
					//guitardb_guitar structure's field

				
				printf("Please input a 14-character-or-less string for the Guitar's make:\n");
				n = 0;
				free(lineptr);
				getline(&lineptr, &n, stdin);	
				strncpy(newGuitar->make, lineptr, 14);
				//If a trailing \n is found, remove it
				char * newline = strstr(newGuitar->make, "\n");
				if (newline)
					newline[0] = '\0';

				printf("Please input a 14-character-or-less string for the Guitar's model:\n");
				n = 0;
				free(lineptr);
				getline(&lineptr, &n, stdin);	
				strncpy(newGuitar->model, lineptr, 14);
				//If a trailing \n is found, remove it
				char * newline = strstr(newGuitar->model, "\n");
				if (newline)
						newline[0] = '\0';

				printf("Please input a 14-character-or-less string for the Guitar's submodel:\n");
				n = 0;
				free(lineptr);
				getline(&lineptr, &n, stdin);	
				strncpy(newGuitar->submodel, lineptr, 14);
				//If a trailing \n is found, remove it
				char * newline = strstr(newGuitar->submodel, "\n");
				if (newline)
					newline[0] = '\0';


				printf("Please input a 2-character-or-less string for the Guitar's body type:\n");
				n = 0;
				free(lineptr);
				getline(&lineptr, &n, stdin);
				strncpy(newGuitar->body_type, lineptr, 2);
					
				// Loop until a valid int input is found
				while (1) {
					printf("Please input an integer for the Guitar's year of manufacture:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", &newGuitar->year))

					break;
				}

				printf("Please input a 24-character-or-less string for the Guitar's finish:\n");
				n = 0;
				free(lineptr);
				getline(&lineptr, &n, stdin);
				strncpy(newGuitar->finish, lineptr, 24);
				//If a trailing \n is found, remove it
				char * newline = strstr(newGuitar->finish, "\n");
				if (newline)
					newline[0] = '\0';

				// Loop until a valid int input is found
				while (1) {
					printf("Please input an integer for the Guitar's strings:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", &newGuitar->strings))
						break;
				}

				// Again, loop until a valid int input is found
				while (1) {
					printf("Please input an integer for the Guitar's pickups:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%d", &newGuitar->pickups))
						break;
				}

				// Again, loop until a valid char input is found
				while (1) {
					printf("Please input a single character for the Guitar's neck pickup:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%c", &newGuitar->neck))
						break;
				}

				// Again, loop until a valid char input is found
				while (1) {
					printf("Please input a single character for the Guitar's middle pickup:\n");
					n = 0;
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%c", &newGuitar->middle))
						break;
				}

				// Again, loop until a valid char input is found
				while (1) {
					printf("Please input a single character for the Guitar's bridge pickup:\n");
					n = 0;	
					free(lineptr);
					getline(&lineptr, &n, stdin);
					if (sscanf(lineptr, "%c", &newGuitar->bridge))
						break;
				}

				//Assign UID
				newGuitar->item_key = numguitars;
		
				if (guitardb_add(guitardb, newGuitar) == 1)
					numguitars++;
			
				else 
					printf("Failed to add guitar - guitar already exists in database.\n");
			} // end if (!srcmp(command, "ADD"))

			else if (!strcmp(command, "LIST"))
			{
				// fetch and list first guitar
				struct guitardb_guitar * listGuitar = guitardb_first(guitardb);
				if (!listGuitar)
				{
					// an error occured - guitardb likely empty
					printf("Error attempting to list guitar database - guitardb is empty.\n");
					continue;
				}
				doListGuitar(listGuitar);

				// iterate through rest of list - list all remaining guitars
				while ((listGuitar = guitardb_next(guitardb)))
				{
					doListGuitar(listGuitar);
				} // end while ((listGuitar = guitardb_next(guitardb))
					
			} // end else if (!strcmp(command, "LIST"))

			else if (!strcmp(command, "QUIT"))
				break;
		} // end if (sscanf(lineptr, "%s %d", command, &inpItemKey));

		else if (sscanf(lineptr, "%s %d", command, &inpItemKey) == 2)
		{ // if two arguments - a string and an int - were matched from menu input
			// either LOOK or DEL was selected

			if (!strcmp(command, "LOOK"))
			{
				struct guitardb_guitar * lookGuitar = 
					guitardb_lookup(guitardb, inpItemKey);
				
				if (!lookGuitar)
				{
					printf("Data not found.\n");
					continue;
				}
	
				printf("\nGuitar #: %d\n", lookGuitar->item_key);
				printf("Make: %s\n", lookGuitar->make);
				printf("Model:%s\n", lookGuitar->model);	
				printf("Submodel: %s\n", lookGuitar->submodel);

				// this is a bad hack to make up for the fact that body_type is 
				// declared as a char array of 2 elements (no NULL termiantor)
				char bodytypeNullTerminated[3];
				strncpy(bodytypeNullTerminated, lookGuitar->body_type, 2);
				bodytypeNullTerminated[2] = '\0'; // append null byte

				// Originally, my program just declared guitardb_guitar.body_type
				// as a 3-element array, and assumed that the intention was to 
				// have a variable that would store 2 letters and a NULL byte

				printf("Body Type: %s\n", bodytypeNullTerminated);
				printf("Year: %d\n", lookGuitar->year);
				printf("Finish: %s\n", lookGuitar->finish);
				printf("Strings: %d\n", lookGuitar->strings);
				printf("Pickups: %d\n", lookGuitar->pickups);
				printf("Neck Pickup: %c\n", lookGuitar->neck);
				printf("Middle Pickup: %c\n", lookGuitar->middle);
				printf("Bridge Pickup: %c\n", lookGuitar->bridge);
			} // end if (!strcmp(command, "LOOK"))

			if (!strcmp(command, "DEL"))
			{
				struct guitardb_guitar * delGuitar = guitardb_lookup(guitardb,
					inpItemKey);
				
				int guitarDelRtrn = guitardb_delete(guitardb, inpItemKey);

				if (guitarDelRtrn  == -2 || guitarDelRtrn == -1)
				{
					printf("Data not found.\n");
					continue;
				}

				//free the guitar we deleted from list
				free(delGuitar);
				delGuitar = NULL;
				printf("Data deleted.\n");		
			} // end 

		} // end else if (sscanf(lineptr, "%s %d", command, &inpItemKey) == 2)
	} // end while(1)

	// free all guitars
	struct guitardb_guitar * destroyGuitar = NULL;
	for (int x = 1; x < numguitars; x++)
	{
		// Search entire list for all item_key numbers that we know we used previously
		if ((destroyGuitar = guitardb_lookup(guitardb, x)))
		{
			//Delete element from list, then free it
			guitardb_delete(guitardb, destroyGuitar->item_key);
			free(destroyGuitar);
			destroyGuitar = NULL;
		}
	}

	free (lineptr);
	guitardb_destroy(guitardb);
	return 0;
}

/*
int doListGuitar(struct guitardb_guitar * listGuitar)
- Simply prints out the truncated attributes of a guitardb_guitar struct in a given format
- struct guitardb_guitar * listGuitar - the guitar struct to be outputted
- returns -1 on fail or 1 on success
*/

int doListGuitar(struct guitardb_guitar * listGuitar)
{
	if (!listGuitar)
		return -1;

	//print item_key, make, model and year
	printf("\nGuitar #: %d\n", listGuitar->item_key);
	printf("Make: %s\n", listGuitar->make);	
	printf("Model: %s\n", listGuitar->model);
	printf("Year: %d\n\n", listGuitar->year);
	return 1;
}
