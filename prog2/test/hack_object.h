/**************************/
/* hack_object.h          */
#ifndef HACK_OBJECT_H
#define HACK_OBJECT_H

/* types for keeping up with inanimate objects */

enum o_type
{
    potion_of_power, potion_of_speed, potion_of_invisibility,
    scroll_of_teleportation, scroll_of_scare_monster,
    wand_of_magic_missel, wand_of_polymorph,
    book_of_healing, book_of_magic_missle,
    knife, black_kknife, short_sword, long_sword, curved_sword,
    iron_helm, cloak_of_power, wood_greaves, mesh_armour,
    gold
};

enum o_class
{
    Tool, Potion, Scroll, Wand, Ring, Food, Gem, Book, Weapon, Armour,
    Amulet, Gold
};

enum o_status
{
    Blessed, Cursed, Normal
};

struct loc
{
    int l;  /** level */
    int x;
    int y;
};

typedef struct h_object_s
{
    int           id;
    struct loc    loc;
    enum o_type   type;
    enum o_class  class;
    enum o_status status;
    int           charge;
    int           weight;
} h_object;

/** Print the string representation of an object.
 *
 * Writes the string representation of OBJECT to STREAM.
 *
 * Returns the number of characters printed (not including the
 * trailing '\0') or -1 on error.
 */
int h_object_print(FILE *stream, const h_object *object);

/** Compare two game objects.
 *
 * Returns an integer greater than, equal to, or less than 0,
 * according as the h_object LEFT is greater than, equal to, or less
 * than the h_object RIGHT.
 */
int h_object_compare(const void *left, const void *right);

#endif
/**************************/
