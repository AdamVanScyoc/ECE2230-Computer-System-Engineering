/*-------------------------*/
/* btree.h */
#ifndef BTREE_H_
#define BTREE_H_

// defines a binary tree data structure that is implemented as an
// array of fixed size

/** pointer to opaque binary tree type to be defined in btree.c */
typedef struct binary_tree_s binary_tree_t;
/**binarytree data pointer, not to be dereferened or freed by the binarytree. */
typedef void *btree_data;


/** Allocates and initializes a new empty binarytree.
 *
 * Returns a new empty binarytree or NULL if there was an error.
 */
binary_tree_t ** binarytree_init(int size);

/* Destroys the binarytree and frees its memory.
 *
 * Frees all memory allocated for the binarytree. binarytree data is not freed.
 */
void binarytree_finalize(binary_tree_t ** finTree, int size);

/* pop's highest priority (the root) node from priority queue
 *
 * re-heapifies itself, returns highest priority node's data
 */
btree_data binarytree_pop(binary_tree_t ** popTree);

#endif  /* BTREE_H_ */
/*-------------------------*/
