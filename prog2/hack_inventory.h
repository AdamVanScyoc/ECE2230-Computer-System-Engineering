/**************************/
/* hack_inventory.h       */
#ifndef HACK_INVENTORY_H_
#define HACK_INVENTORY_H_

/** pointer to opaque inventory type to be defined in hack_inventory.c */
typedef struct h_inventory *h_inventory;

/** Object comparison function.
 *
 * An object comparison function shall compare the two objects LEFT
 * and RIGHT. If LEFT compares equal to RIGHT then it returns zero,
 * otherwise it returns nonzero.
 */
typedef int (*h_inventory_cmp_f)(const h_object *left, const h_object *right);

/** Creates a new empty inventory.
 *
 * The current object is initially NULL.
 *
 * Returns the new inventory or NULL on error.
*/
h_inventory h_inventory_create(void);

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
h_object *h_inventory_add(h_inventory inventory, h_object *object);

/** Removes the current object from the inventory.
 *
 * The new current object becomes the object after the current object
 * (or NULL if there is no object after the current object).
 *
 * Returns the object that was removed or NULL if the current object
 * was NULL.
 */
h_object *h_inventory_delete(h_inventory inventory);

/** Move to the first object in the inventory.
 *
 * Sets the current object to the first object in the inventory (or
 * NULL if the inventory is empty).
 *
 * Returns the first object in the inventory or NULL if the inventory
 * is empty.
 */
h_object *h_inventory_first(h_inventory inventory);

/** Move to the next object in the inventory.
 *
 * Sets the new current object to the object after the current
 * object. There is no object after the NULL current object.
 *
 * Returns the object after the current object or NULL if there is no
 * object after the current object.
 */
h_object *h_inventory_next(h_inventory inventory);

/** Finds an object by its id.
 *
 * Sets the current object to the object whose id is equal to ID (or
 * NULL if no such object was found).
 *
 * Returns the matching object or NULL if no such object was found.
 */
h_object *h_inventory_find_id(h_inventory inventory, int id);

/** Finds an object using the provided comparison function.
 *
 * The search begins at the current object and continues until OBJCMP
 * returns 0 or until the end of the inventory. If the current object
 * was NULL then the search begins at the first object in the
 * inventory.
 *
 * The current object is set to the first object for which OBJCMP
 * returns 0, and then the search stops. If the search reaches the end
 * before OBJCMP returns 0 then the current object is set to NULL.
 *
 * Returns the first object for which OBJCMP returns 0 or NULL if no
 * such object was found.
 */
h_object *h_inventory_find_func(h_inventory inventory, h_inventory_cmp_f objcmp);

/** Destroys the inventory and frees its memory.
 *
 * Storage for any objects in the inventory is never freed.
 */
void h_inventory_destroy(h_inventory inventory);

#endif  /* HACK_INVENTORY_H_ */
/**************************/
