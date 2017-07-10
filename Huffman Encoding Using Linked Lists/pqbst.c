/*=============================================================================
|  Author: Jithya Nanayakkara
|  userid: vbjdn
|  Last Modified: 14/11/2011
+------------------------------------------------------------------------------
|Description: 
| This file declares the data structures and functions for creating and
| managing a Binary Search Tree priority queue.
|
| The structs defined are for:
|  - A BST node, with pointers to two child nodes, its parent and a huffman node
|
| The functions can:
|  - Insert a new node into the BST.
|  - Allocate a new BST node.
|  - Remove the BST node with the lowest huffman node frequency.
|  - Find the size of the BST.
+============================================================================*/

#include "pqbst.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>


/* Auxiliary function declarations */
static bstNode*
bst_minimum (bstNode* bstN) ;

static void
transplant (bstNode** root, bstNode* u, bstNode* v) ;

static void
delete_bst_node (bstNode** root, bstNode* z) ;


/* Functions */

/*-----------------------------------------------------------------------------
| Function: create_bst_node 
|
| Description:
|  Allocates a new priority queue BST node, that stores the location of a 
|  huffman node.
|
| Parameters:
|  - newHuffNode: A pointer to the address of the huffman node you wish to 
|                 insert into the priority queue.
|  -        left: A pointer to the left BST child.
|  -       right: A pointer to the right BST child.
|  -      parent: A pointer to this node's parent BST noe.
|
| Returns: A pointer to the newly allocated bstNode.
*----------------------------------------------------------------------------*/
bstNode* 
create_bst_node 
(huffNode** newHuffNode, bstNode* left, bstNode* right, bstNode* parent)
{
   bstNode* newNode = (bstNode*) safe_malloc (sizeof(bstNode)) ;
   
   if (newHuffNode == NULL) {
     fprintf (stderr, 
      "Error: Can't create a priority queue node without a huffman node.\n") ;
     exit(EXIT_FAILURE) ;
   }
   
   newNode->hNode = (*newHuffNode) ;
   newNode->left  = left ;
   newNode->right = right ;
   newNode->parent = parent ;
   
   return newNode ;
}


/*-----------------------------------------------------------------------------
| Function: insert_bstnode
|
| Description:
|   Insert a BST node into the tree (in the correct location).
|
| Algorithm:
|  Look for a NULL node to replace with the new node.
|  Which direction to traverse is determined by comparing the frequencies
|  of the new node and the node being traversed.
|  A trailing pointer y is maintained as the parent of x, and is needed
|  to set the parent of the new node and to set the new node as y's child.
|
| Parameters:
|   newNode - pointer to a BST node to insert.
|      root - double pointer to the root of the BST.
*----------------------------------------------------------------------------*/
void
insert_bstnode
(bstNode** root, bstNode* newNode)
{
   bstNode* y = NULL ;
   bstNode* x = *root ;
   
   while (x != NULL) {
      y = x ;
      
      if (newNode->hNode->freq < x->hNode->freq) {
         x = x->left ;
      }
      else {
         x = x->right ;
      }
   }
   
   newNode->parent = y ;
   
   if (y == NULL) {
      /* If Tree is empty */
      (*root) = newNode ;
   }
   else if (newNode->hNode->freq < y->hNode->freq) {
      y->left = newNode ;
   }
   else {
      y->right = newNode ;
   }
}



/*-----------------------------------------------------------------------------
| Function: bst_size
|
| Description:
|   Calculate the number of nodes in the bst.
|
| Algorithm:
|  Each successful traversal of the tree adds 1 to the final return
|  value.
|
| Parameters:
|   bstN - a subtree 
|
| Returns: The size of the BST
*----------------------------------------------------------------------------*/
unsigned int
bst_size 
(bstNode* bstN)
{
   if (bstN == NULL) {
      return 0 ;
   }
   else {
      return ( bst_size(bstN->left) + 1 + bst_size(bstN->right) ) ;
   }
}



/*-----------------------------------------------------------------------------
| Function: remove_priority_bstnode
|
| Description:
|   Remove the BST node with the lowest key and return the huffman node it
|   points to.
|
| Algorithm:
|  Find the minimum key node to remove.
|  Retrieve the huffman node from that BST node.
|  Delete that BST node from the tree.
|
| Parameters:
|   root - a double pointer to the root of the tree.
|
| Returns: The removed huffman node
*----------------------------------------------------------------------------*/
huffNode*
remove_priority_bstnode
(bstNode** root)
{
   bstNode* nodeToDelete = NULL ;
   huffNode* hNode = NULL ;

   nodeToDelete = bst_minimum (*root) ;
   hNode = nodeToDelete->hNode ;
   delete_bst_node (root, nodeToDelete) ;
   free (nodeToDelete) ;
   
   return hNode ;
}

/*-----------------------------------------------------------------------------
| Function: bst_minimum
|           (Auxiliary function to remove_priority_bstnode)
|
| Description:
|   Returns the BST node with the lowest frequency key.
|
| Algorithm:
|  Simply keep traversing the left branch of the tree untill you reach
|  the end. The property of BST means that node will have the lowest value.
|
| Parameters:
|   bstN - pointer to a subtree of a BST
|
| Returns: A pointer to the BST node with the lowest frequency.
*----------------------------------------------------------------------------*/
static bstNode*
bst_minimum
(bstNode* bstN)
{
   while (bstN->left != NULL) {
      bstN = bstN->left ;
   }
   
   return bstN ;
}

/*-----------------------------------------------------------------------------
| Function: delete_bst_node
|           (Auxiliary function to remove_priority_bstnode)
| 
| Deletes a node z from the BST while maintaining the structure
| of a BST.
*----------------------------------------------------------------------------*/
static void
delete_bst_node 
(bstNode** root, bstNode* z)
{
   bstNode* y ;
   
   if (z->left == NULL) {
      transplant (root, z, z->right) ;
   }
   else if (z->right == NULL) {
      transplant (root, z, z->left) ;
   }
   else {
      y = bst_minimum (z->right) ;
      
      if (y->parent != z) {
         transplant (root, y, y->right) ;
         y->right = z->right ;
         y->right->parent = y ;
      }
      
      transplant (root, z, y) ;
      y->left = z->left ;
      y->left->parent = y ;
   }
}

/*-----------------------------------------------------------------------------
| Function: transplant
|           (Auxiliary function to delete_bst_node)
| 
| Replaces a subtree rooted at node u with the subtree rooted at node v.
*----------------------------------------------------------------------------*/ 
static void
transplant 
(bstNode** root, bstNode* u, bstNode* v)
{
   if (u->parent == NULL) {
      (*root) = v ;
   }
   else if (u == u->parent->left) {
      u->parent->left = v ;
   }
   else {
      u->parent->right = v ;
   }
   
   if (v != NULL) {
      v->parent = u->parent ;
   }
}


/*=============================================================================
||                               DEBUG FUNCTIONS                             ||
=============================================================================*/


/*-----------------------------------------------------------------------------
| Function: inorder_tree_walk
|
|  (Debug Function)
|  Prints the BST nodes in ascending order.
*----------------------------------------------------------------------------*/
void
inorder_tree_walk
(bstNode* bstN) 
{
   if (bstN != NULL) {
      inorder_tree_walk (bstN->left) ;
      printf ("%d ", bstN->hNode->freq) ;
      inorder_tree_walk (bstN->right) ;
   }
}

