/*-------------------------*/
/* btree.c */

// implements a binary tree data structure as an array of fixed size

#include <stdlib.h>

#include "btree.h"


// Structure that describes a single node of binary tree
typedef struct btreenode
{
	btree_data data;
	struct btreenode * right;
	struct btreenode * left;
} btreenode_t;

struct binary_tree_s 
{
	int numnodes;
	
};

/** Allocates and initializes a new empty binarytree.
 *
 * Returns a new empty binarytree or NULL if there was an error.
 */
binary_tree_t ** binarytree_init(int size)
{
	if (size <= 0)
		return NULL;
	
	// + 1 because first element of array storing binary tree is always empty
	binary_tree_t ** newTree = calloc(size + 1, sizeof(*newTree));
	if (!newTree)
		return NULL;

	newTree[0] = NULL;

	return newTree;
}

/* Destroys the binarytree and frees its memory.
 *
 * Frees all memory allocated for the binarytree. binarytree data is not freed.
 */
void binarytree_finalize(binary_tree_t ** finTree, int size)
{
	if (!finTree || size <= 0)
		return;

	for (int x = 0; x < size; x++)
	{
		if (finTree[x])
		{
			free(finTree[x]);
			finTree[x] = NULL;
		}
	}

	free(finTree);
	finTree = NULL;

	return;
}


/* pop's highest priority (the root) node from priority queue
 *
 * re-heapifies itself, returns highest priority node's data
 */
btree_data binarytree_pop(binary_tree_t ** popTree)
{
	if (!popTree)
		return NULL;

	// check that root exists; otherwise btree is empty
	if (!popTree[1])	
		return NULL;

	
}

/*-------------------------*/
