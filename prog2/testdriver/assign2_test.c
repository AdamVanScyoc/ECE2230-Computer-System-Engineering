#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "list.h"
#include "hack_object.h"
#include "hack_inventory.h"

////////////////////////////////////////////////////////////////////////
// Global objects used during tests
////////////////////////////////////////////////////////////////////////
 
// first unique object
static h_object obj1 = {
  .id = 2442,
  .loc = { 1, 2, 3 },
  .type = potion_of_power,
  .class = Potion,
  .status = Normal,
  .charge = 1,
  .weight = 1
};

// second unique object
static h_object obj2 = {
  .id = 8487,
  .loc = {10, 10, 1},
  .type = cloak_of_power,
  .class = Armour,
  .status = Cursed,
  .charge = 1,
  .weight = 3
};

// third object will be initialized in main() to a copy of obj2
static h_object obj3;

// fourth unique object
static h_object obj4 = {
  .id = 8317,
  .loc = { 5, 3, 5 },
  .type = book_of_healing,
  .class = Book,
  .status = Blessed,
  .charge = 0,
  .weight = 4
};

// fifth object shares the same key as obj2
static h_object obj5 = {
  .id = 8487,  // same as obj2
  .loc = { 1, 1, 1 },
  .type = scroll_of_teleportation,
  .class = Scroll,
  .status = Normal,
  .charge = 3,
  .weight = 1
};

// sixth unique object with the same class as obj4
static h_object obj6 = {
  .id = 5150,
  .loc = {8, 6, 17},
  .type = book_of_magic_missle,
  .class = Book,
  .status = Cursed,
  .charge = 1,
  .weight = 7
};

// will store backup copies of all objects
static h_object obj1_cp, obj2_cp, obj3_cp, obj4_cp, obj5_cp, obj6_cp;

////////////////////////////////////////////////////////////////////////
// Object test functions
////////////////////////////////////////////////////////////////////////

// test the object print function
static void
object_print_test(void)
{
  int rc = -1;

  // output to /dev/null to avoid interfering with test status info
  // r+ to prevent the file from being created if it doesn't exist
  FILE *stdnull = fopen("/dev/null", "r+");
  if (stdnull) {
    // no easy way to test print so just check for non-error (-1)
    rc = h_object_print(stdnull, &obj1);
  } else {
    // guess we have to skip this test
    // I hope you're not using Windows!
    perror("fopen /dev/null");
    rc = 0;
  }
  CU_ASSERT_NOT_EQUAL(rc, -1);
}

// test the object comparison function
static void
object_compare_test(void)
{
  int rc = -1;

  // compare identical objects (same memory address)
  rc = h_object_compare(&obj1, &obj1);
  CU_ASSERT_EQUAL(rc, 0);

  // compare equal objects (different memory, same contents)
  rc = h_object_compare(&obj2, &obj3);
  CU_ASSERT_EQUAL(rc, 0);

  // compare different objects
  rc = h_object_compare(&obj1, &obj2);
  CU_ASSERT_NOT_EQUAL(rc, 0);

  // compare objects sharing and id (but the rest is different)
  rc = h_object_compare(&obj2, &obj5);
  CU_ASSERT_NOT_EQUAL(rc, 0);
}

////////////////////////////////////////////////////////////////////////
// List test functions
////////////////////////////////////////////////////////////////////////

// test operations on an empty list
static void
list_empty_test(void)
{
  list_data data = NULL;
  list lst = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst);

  // current item is NULL in a new list
  data = list_curr(lst);
  CU_ASSERT_PTR_NULL(data);

  // nothing to remove
  data = list_remove(lst);
  CU_ASSERT_PTR_NULL(data);

  // can't insert_before if the current item is NULL
  data = list_insert_before(lst, &obj1);
  CU_ASSERT_PTR_NULL(data);

  // can't insert_after if the current item is NULL;
  data = list_insert_after(lst, &obj2);
  CU_ASSERT_PTR_NULL(data);

  // no first because the list is empty
  data = list_first(lst);
  CU_ASSERT_PTR_NULL(data);

  // no next because the list is empty
  data = list_next(lst);
  CU_ASSERT_PTR_NULL(data);

  // still no next
  data = list_next(lst);
  CU_ASSERT_PTR_NULL(data);

  // no prev because the list is empty
  data = list_prev(lst);
  CU_ASSERT_PTR_NULL(data);

  // no last because the list is empty
  data = list_last(lst);
  CU_ASSERT_PTR_NULL(data);

  list_finalize(lst);
  lst = NULL;
}

// test inserting and appending to a list
static void
list_insert_append_test(void)
{
  list_data data = NULL;
  list lst = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst);

  // insert obj1 at the head of an empty list
  // before: [ ]
  data = list_insert(lst, &obj1);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  // after: [ obj1 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  // append obj2 to the tail
  // before: [ obj1 ]
  data = list_append(lst, &obj2);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  // after: [ obj1 obj2 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);

  // insert obj3 at the head
  // before: [ obj1 obj2 ]
  data = list_insert(lst, &obj3);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  // after: [ obj3 obj1 obj2 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);

  // append obj4 to the tail
  // before: [ obj3 obj1 obj2 ]
  data = list_append(lst, &obj4);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  // after: [ obj3 obj1 obj2 obj4 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj4);

  // get a new empty list
  // before: [ obj3 obj1 obj2 obj4 ]
  list_finalize(lst);
  lst = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst);
  // after: [ ]

  // insert obj1 at the tail of an empty list
  // before: [ ]
  data = list_append(lst, &obj1);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  // after: [ obj1 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  list_finalize(lst);
  lst = NULL;
}

// test inserting before and after the current object
static void
list_insert_before_after_test(void)
{
  list_data data = NULL;
  list lst = list_init();

  // put obj1 in the list (setup)
  // before: [ ]
  (void)list_insert(lst, &obj1);
  // after: [ obj1 ]

  // insert before the head
  // before: [ obj1 ]
  (void)list_first(lst);
  data = list_insert_before(lst, &obj2);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  // after: [ obj2 obj1 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  // insert after the head
  (void)list_first(lst);
  // before: [ obj2 obj1 ]
  data = list_insert_after(lst, &obj3);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  // after: [ obj2 obj3 obj1 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  // insert before the tail
  (void)list_last(lst);
  // before: [ obj2 obj3 obj1 ]
  data = list_insert_before(lst, &obj4);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  // after: [ obj2 obj3 obj4 obj1 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  // insert after the tail
  (void)list_last(lst);
  // before: [ obj2 obj3 obj4 obj1 ]
  data = list_insert_after(lst, &obj5);
  CU_ASSERT_PTR_EQUAL(data, &obj5);
  // after: [ obj2 obj3 obj4 obj1 obj5 ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj5);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj5);

  // iterate and check the whole list
  // checking for [ obj2 obj3 obj4 obj1 obj5 ]
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj5);

  list_finalize(lst);
  lst = NULL;
}

// test iterating forward and backward through a list
static void
list_iterate_test(void)
{
  list_data data = NULL;
  list lst = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst);

  // first add some items to the list (setup)
  // before: [ ]
  (void)list_append(lst, &obj1);
  (void)list_append(lst, &obj2);
  (void)list_append(lst, &obj3);
  // after: [ obj1 obj2 obj3 ]

  // go through in forward order
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_next(lst);
  CU_ASSERT_PTR_NULL(data);

  // go through in reverse order
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_prev(lst);
  CU_ASSERT_PTR_NULL(data);

  // go forward then back
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_prev(lst);
  CU_ASSERT_PTR_NULL(data);

  // go back then forward
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_prev(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_next(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_next(lst);
  CU_ASSERT_PTR_NULL(data);

  list_finalize(lst);
  lst = NULL;
}

// test removing items from a list
static void
list_remove_test(void)
{
  list_data data = NULL;
  list lst = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst);

  // first add some items to the list (setup)
  // before: [ ]
  (void)list_append(lst, &obj1);
  (void)list_append(lst, &obj2);
  (void)list_append(lst, &obj3);
  // after: [ obj1 obj2 obj3]

  // delete the middle item
  (void)list_first(lst);
  (void)list_next(lst);
  // before: [ obj1 obj2 obj3 ]
  data = list_remove(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  // after: [ obj1 obj3(current) ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);

  // delete the first item
  (void)list_first(lst);
  // before: [ obj1 obj3 ]
  data = list_remove(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  // after: [ obj3(current) ]
  data = list_curr(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_first(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  data = list_last(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);

  // delete the last remaining item
  (void)list_last(lst);
  // before: [ obj3 ]
  data = list_remove(lst);
  CU_ASSERT_PTR_EQUAL(data, &obj3);
  // after: [ ]
  data = list_curr(lst);
  CU_ASSERT_PTR_NULL(data);
  data = list_first(lst);
  CU_ASSERT_PTR_NULL(data);
  data = list_last(lst);
  CU_ASSERT_PTR_NULL(data);
  
  list_finalize(lst);
  lst = NULL;
}

// test independence of different lists
static void
list_multiple_test(void)
{
  list_data data = NULL;
  list lst1 = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst1);
  list lst2 = list_init();
  CU_ASSERT_PTR_NOT_NULL_FATAL(lst2);

  // add some items to the first list (setup)
  // before: [ ]
  (void)list_append(lst1, &obj1);
  (void)list_append(lst1, &obj2);
  (void)list_append(lst1, &obj3);
  // after: [ obj1 obj2 obj3]

  // add some items to the second list (setup)
  // before: [ ]
  (void)list_append(lst2, &obj4);
  (void)list_append(lst2, &obj5);
  (void)list_append(lst2, &obj1);
  // after: [ obj4 obj5 obj1]

  // check the first list
  data = list_first(lst1);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  data = list_next(lst1);
  CU_ASSERT_PTR_EQUAL(data, &obj2);
  data = list_next(lst1);
  CU_ASSERT_PTR_EQUAL(data, &obj3);

  // check the second list
  data = list_first(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj5);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  // remove from the first list (setup)
  (void)list_first(lst1);
  (void)list_remove(lst1);

  // check the second list again after modifying the first list
  data = list_first(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj5);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj1);
  
  // destroy the first list (setup)
  list_finalize(lst1);
  lst1 = NULL;

  // check the second list again after destroying the first list
  data = list_first(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj4);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj5);
  data = list_next(lst2);
  CU_ASSERT_PTR_EQUAL(data, &obj1);

  list_finalize(lst2);
  lst2 = NULL;
}

////////////////////////////////////////////////////////////////////////
// Inventory test functions
////////////////////////////////////////////////////////////////////////

// test operations on an empty inventory
static void
inventory_empty_test(void)
{
  h_object * obj = NULL;
  h_inventory inv = h_inventory_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(inv);

  // nothing to delete
  obj = h_inventory_delete(inv);
  CU_ASSERT_PTR_NULL(obj);

  // no first because inventory is empty
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_NULL(obj);

  // no next because inventory is empty
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_NULL(obj);

  // still no next
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_NULL(obj);

  // nothing to find
  obj = h_inventory_find_id(inv, 6345);
  CU_ASSERT_PTR_NULL(obj);

  h_inventory_destroy(inv);
  inv = NULL;
}

// test adding objects to an inventory
static void
inventory_add_test(void)
{
  h_object *obj = NULL;
  h_inventory inv = h_inventory_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(inv);

  // add a potion
  // before: [ ]
  obj = h_inventory_add(inv, &obj1);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  // after: [ obj1 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);

  // add an armour (goes after the potion)
  // before: [ obj1 ]
  obj = h_inventory_add(inv, &obj2);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);
  // after: [ obj1 obj2 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);
  
  // add a book (goes between the potion and the armour)
  // before: [ obj1 obj2 ]
  obj = h_inventory_add(inv, &obj4);
  CU_ASSERT_PTR_EQUAL(obj, &obj4);
  // after: [ obj1 obj4 obj2 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj4);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);

  // add another book (goes after the first book)
  // before: [ obj1 obj4 obj2 ]
  obj = h_inventory_add(inv, &obj6);
  CU_ASSERT_PTR_EQUAL(obj, &obj6);
  // after: [ obj1 obj4 obj6 obj2 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj4);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj6);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);

  // try to add an object with a duplicate key
  obj = h_inventory_add(inv, &obj5);
  CU_ASSERT_PTR_NULL(obj);
  
  h_inventory_destroy(inv);
  inv = NULL;
}

// test finding an object by its id
static void
inventory_find_test(void)
{
  h_object *obj = NULL;
  h_inventory inv = h_inventory_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(inv);

  // add some items (setup)
  // before: [ ]
  (void)h_inventory_add(inv, &obj1);
  (void)h_inventory_add(inv, &obj4);
  (void)h_inventory_add(inv, &obj6);
  (void)h_inventory_add(inv, &obj2);
  // after: [ obj1 obj4 obj6 obj2 ]

  // try to find every item by its id
  obj = h_inventory_find_id(inv, obj6.id);
  CU_ASSERT_PTR_EQUAL(obj, &obj6);
  obj = h_inventory_find_id(inv, obj4.id);
  CU_ASSERT_PTR_EQUAL(obj, &obj4);
  obj = h_inventory_find_id(inv, obj1.id);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  obj = h_inventory_find_id(inv, obj2.id);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);

  // test the next item after the found item
  (void)h_inventory_find_id(inv, obj4.id);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj6);

  // should be no "next" if the item wasn't found
  (void)h_inventory_find_id(inv, 999);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_NULL(obj);

  h_inventory_destroy(inv);
  inv = NULL;
}

// test deleting objects from an inventory
static void
inventory_delete_test(void)
{
  h_object *obj = NULL;
  h_inventory inv = h_inventory_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(inv);

  // add some items (setup)
  // before: [ ]
  (void)h_inventory_add(inv, &obj1);
  (void)h_inventory_add(inv, &obj4);
  (void)h_inventory_add(inv, &obj6);
  (void)h_inventory_add(inv, &obj2);
  // after: [ obj1 obj4 obj6 obj2 ]

  // delete from the middle, twice
  (void)h_inventory_first(inv);
  (void)h_inventory_next(inv);
  // before: [ obj1 obj4 obj6 obj2 ]
  obj = h_inventory_delete(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj4);
  // then: [ obj1 obj6 obj2 ]
  obj = h_inventory_delete(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj6);
  // after: [ obj1 obj2 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  obj = h_inventory_next(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);

  // delete the first item
  (void)h_inventory_first(inv);
  // before: [ obj1 obj2 ]
  obj = h_inventory_delete(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj1);
  // after: [ obj2 ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);

  // delete the last item
  (void)h_inventory_first(inv);
  // before: [ obj2 ]
  obj = h_inventory_delete(inv);
  CU_ASSERT_PTR_EQUAL(obj, &obj2);
  // after: [ ]
  obj = h_inventory_first(inv);
  CU_ASSERT_PTR_NULL(obj);

  h_inventory_destroy(inv);
  inv = NULL;
}

////////////////////////////////////////////////////////////////////////
// Test driver
////////////////////////////////////////////////////////////////////////

// make sure no objects were modified during a test
static void
object_modification_test(void)
{
  // check every object against its original value
  int diff = memcmp(&obj1, &obj1_cp, sizeof(obj1));
  CU_ASSERT_EQUAL(diff, 0);
  diff = memcmp(&obj2, &obj2_cp, sizeof(obj2));
  CU_ASSERT_EQUAL(diff, 0);
  diff = memcmp(&obj3, &obj3_cp, sizeof(obj3));
  CU_ASSERT_EQUAL(diff, 0);
  diff = memcmp(&obj4, &obj4_cp, sizeof(obj4));
  CU_ASSERT_EQUAL(diff, 0);
  diff = memcmp(&obj5, &obj5_cp, sizeof(obj5));
  CU_ASSERT_EQUAL(diff, 0);
  diff = memcmp(&obj6, &obj6_cp, sizeof(obj6));
  CU_ASSERT_EQUAL(diff, 0);
}

// restore objects to their original value in case some misbehaving
// function modified them
static int
reset_objects(void)
{
  // restore from backups
  memcpy(&obj1, &obj1_cp, sizeof(obj1));
  memcpy(&obj2, &obj2_cp, sizeof(obj2));
  memcpy(&obj3, &obj3_cp, sizeof(obj3));
  memcpy(&obj4, &obj4_cp, sizeof(obj4));
  memcpy(&obj5, &obj5_cp, sizeof(obj5));
  memcpy(&obj6, &obj6_cp, sizeof(obj6));

  return 0;  // success
}

int
main()
{
  // make obj3 a copy of obj2
  obj3 = obj2;

  // make backup copies of all object storage
  memcpy(&obj1_cp, &obj1, sizeof(obj1_cp));
  memcpy(&obj2_cp, &obj2, sizeof(obj2_cp));
  memcpy(&obj3_cp, &obj3, sizeof(obj3_cp));
  memcpy(&obj4_cp, &obj4, sizeof(obj4_cp));
  memcpy(&obj5_cp, &obj5, sizeof(obj5_cp));
  memcpy(&obj6_cp, &obj6, sizeof(obj6_cp));
  
  CU_ErrorCode cuerr = CU_initialize_registry();
  assert(CUE_SUCCESS == cuerr);

  CU_pSuite object_suite = CU_add_suite("object", reset_objects, reset_objects);
  assert(object_suite);
  CU_ADD_TEST(object_suite, object_print_test);
  CU_ADD_TEST(object_suite, object_compare_test);
  CU_ADD_TEST(object_suite, object_modification_test);

  CU_pSuite list_suite = CU_add_suite("list", reset_objects, reset_objects);
  assert(list_suite);
  CU_ADD_TEST(list_suite, list_empty_test);
  CU_ADD_TEST(list_suite, list_insert_append_test);
  CU_ADD_TEST(list_suite, list_iterate_test);
  CU_ADD_TEST(list_suite, list_remove_test);
  CU_ADD_TEST(list_suite, list_multiple_test);
  CU_ADD_TEST(list_suite, list_insert_before_after_test);
  CU_ADD_TEST(list_suite, object_modification_test);

  CU_pSuite inventory_suite = CU_add_suite("inventory", reset_objects, reset_objects);
  assert(inventory_suite);
  CU_ADD_TEST(inventory_suite, inventory_empty_test);
  CU_ADD_TEST(inventory_suite, inventory_add_test);
  CU_ADD_TEST(inventory_suite, inventory_find_test);
  CU_ADD_TEST(inventory_suite, inventory_delete_test);
  CU_ADD_TEST(inventory_suite, object_modification_test);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  CU_cleanup_registry();
  return EXIT_SUCCESS;
}
