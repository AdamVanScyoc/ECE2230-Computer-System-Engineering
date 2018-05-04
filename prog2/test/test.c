/**************************/
/* test.c                 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hack_object.h"
#include "list.h"
#include "hack_inventory.h"

int main()
{
	// Testing functions in hack_inventory.c
	h_inventory newInv = h_inventory_create();

	h_object * object0 = malloc(sizeof(struct h_object_s));
	
	object0->id = 0;
	object0->loc.l = 0;
	object0->loc.x = 0;
	object0->loc.y = 0;
	object0->type = potion_of_power;
	object0->class = Armour;
	object0->status = Blessed;
	object0->charge = 0;
	object0->weight = 0;

	// test adding a single object to empty inventory
	if (!h_inventory_add(newInv, object0))
		printf("Error adding object0\n");

	h_object * object1 = malloc(sizeof(struct h_object_s));

	object1->id = 1;
	object1->loc.l = 1;
	object1->loc.x = 1;
	object1->loc.y = 1;
	object1->type = potion_of_power;
	object1->class = Weapon;
	object1->status = Blessed;
	object1->charge = 1;
	object1->weight = 1;

	// test adding an object to inventory that contains 1 object
	if (!h_inventory_add(newInv, object1))
		printf("Error adding object1\n");

	h_object * object2 = malloc(sizeof(struct h_object_s));

	object2->id = 2;
	object2->loc.l = 2;
	object2->loc.x = 2;
	object2->loc.y = 2;
	object2->type = potion_of_power;
	object2->class = Book;
	object2->status = Blessed;
	object2->charge = 2;
	object2->weight = 2;

	// test adding a object to inventory that contains 2 objects
	if (!h_inventory_add(newInv, object2))
		printf("Error adding object2\n");

	h_object * object3 = malloc(sizeof(struct h_object_s));

	object3->id = 3;
	object3->loc.l = 3;
	object3->loc.x = 3;
	object3->loc.y = 3;
	object3->type = potion_of_power;
	object3->class = Gem;
	object3->status = Blessed;
	object3->charge = 3;
	object3->weight = 3;

	// test adding a 4th  object to inventory
	if (!h_inventory_add(newInv, object3))
		printf("Error adding object2\n");

	// test list_insert_after() with cursor positioned at first node in list
	//list_insert_after(newList, object3);

	h_inventory_first(newInv);
	h_inventory_next(newInv);
	h_inventory_delete(newInv);
	h_inventory_delete(newInv);

	h_inventory_destroy(newInv);

	printf("%d %d %d %d %d %d %d %d %d %d %d %d \n", Tool, Potion, Scroll, Wand, Ring, Food, Gem, Book, Weapon, Armour, Amulet, Gold);
	
	return 0;
}
