/*=============================================================================
|  Author: Jithya Nanayakkara
|  userid: vbjdn
|  Last Modified: 14/11/2011
+------------------------------------------------------------------------------
|Description: 
| This file defines the data structures and functions for creating and
| managing a huffman tree which holds ascii characters and their frequencies.
|
| The structs defined in huffmantree.h are for:
|  - A huffman node, which points to two child nodes, holds a key (which is
|    the frequency of the ascii character) and the ascii character.
|
| The functions can:
|  - Find the number of nodes in the huffman tree. (for debugging)
|  - Calculate the length of the longest path from the root.
|  - Print the huffman tree (or individual blocks). (for debugging)
|  - Create a node for the huffman tree.
|  - Print the paths in the huffman tree. (for debugging)
|  - Generate a codeword for a given ascii value stored in the huffman tree.
+============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "huffmantree.h"
#include "util.h"

/*Auxiliary Function Declarations*/
static void printArray 
   (huffNode* path, unsigned int pathLen) ;
   
static void print_paths_prime 
   (huffNode* hNode, huffNode* path, int pathLen) ;
   
static void generate_code_bit_list 
   (unsigned int* codeword, unsigned int edge, qHead* codeBitList) ;
   
static void build_codeword 
   (huffNode* hNode, unsigned int ascii, qHead* codeBitList, 
         unsigned int* codeword, unsigned int edge, short int direction) ;



/* Functions */

/*-----------------------------------------------------------------------------
| Function: create_huffnode
|
| Description:
|  Allocate a new huffman tree node.
|  These nodes MUST have a frequency and must satisfy the full binary tree
|  property of huffman trees by:
|  - not pointing to any nodes (thereby being a leaf node) 
|  - OR both its left and right fields must point to another node.
|
| Algorithm:
|  Check if a value for the frequency has been given and if the node satisfies
|  the full binary tree property. Terminate with error if both those checks 
|  have not been satisfied.
|  If satisfied, allocate memory and set the fields of the huffman node to the 
|  corresponding values of the arguments passed to the function.
|
| Parameters:
|  -      freq: The frequency of the node. This cannot be NULL.
|  - character: The ascii value of the character the node holds. 
|  -      left: A pointer to another huffman tree node. 
|  -     right: A pointer to another huffman tree node. 
|
| Returns: A pointer to the newly created huffman node.
*----------------------------------------------------------------------------*/
huffNode* 
create_huffnode 
(unsigned int freq, unsigned int character, huffNode* left, huffNode* right)
{
   huffNode* newHuffNode ;
   
   if (freq == NULL) {
      fprintf (stderr, 
         "Error: Frequency of the huffman node hasn't been specified.") ;
      exit(EXIT_FAILURE) ;
   }
   else if  (((left != NULL) && (right == NULL)) || 
                           ((left == NULL) && (right != NULL))) {
      fprintf (stderr, 
            "Error: The given node must be a leaf or point to two nodes.") ;
      exit(EXIT_FAILURE) ;
   }
   
   newHuffNode = (huffNode*) safe_malloc (sizeof(huffNode)) ;
   newHuffNode->freq = freq ;
   newHuffNode->character = character ;
   newHuffNode->left = left ;
   newHuffNode->right = right ;
   
   return newHuffNode ;
}



/*-----------------------------------------------------------------------------
| Function: traverse_tree
|
| Description:
|  Traverse a huffman tree one node at a time, based on the bit value sent:
|  1 means return the node right of the input node, 0 means left.
|
| Parameters:
|  -   hNode: A pointer of the huffman node you wish to traverse FROM.
|  -     bit: A binary bit, in order to determine the direction of traversal
|
| Returns: A pointer to the huffman node the function traversed to.
*----------------------------------------------------------------------------*/
huffNode*
traverse_tree 
(huffNode* hNode, unsigned short int bit)
{
   if (bit == 0) {
      return (hNode->left) ;
   }
   else if (bit == 1) {
      return (hNode->right) ;
   }
   else {
      fprintf (stderr, 
      "Error: A binary bit has not been passed to function traverse_tree.\n") ;
      exit (EXIT_FAILURE) ;
   }
}



/*-----------------------------------------------------------------------------
| Function: get_max_depth
|
| Description:
|  Finds the number of nodes along the longest path of a given huffman tree,
|  from the root to the farthest leaf node.
|
| Algorithm:
|  Recursively computes the depth of each side of the tree, returning only
|  the side with the larger depth.
|
| Parameters:
|  - hNode: A pointer to the root of the huffman tree.
|
| Returns: The length of the longest path in the huffman tree.
*----------------------------------------------------------------------------*/
unsigned int
get_max_depth
(huffNode* hNode)
{
   unsigned int leftDepth ;
   unsigned int rightDepth ;
   
   if (hNode == NULL) {
      return 0 ;
   }
   else {
      /* compute the depth of each subtree */
      leftDepth = get_max_depth (hNode->left) ;
      rightDepth = get_max_depth (hNode->right) ;
      
      /* use the larger one */
      if (leftDepth > rightDepth) {
         return (leftDepth + 1) ;
      }
      else {
         return (rightDepth + 1) ;
      }
   }
}
      


/*-----------------------------------------------------------------------------
| Function: generate_codeword
|
| Description:
|  Generates a the huffman codeword for a given ascii value.
|
| Algorithm:
|  !See the auxiliary functions, build_codeword and generate_code_bit_list
|  !for information.
|   Note that the 5th and 6th arguments passed to build_codeword signify the
|   node being passed to it is a root node.
|
| Parameters:
|  - huffmanTreeRoot: A pointer to the root
|  -           ascii: The ascii value that needs to be encoded.
|  -     codeBitList: The head of a queue that stores the huffman bits 
|                     as a linked list.
|  -          length: The length of the codeword.
*----------------------------------------------------------------------------*/
void
generate_codeword 
(huffNode* huffmanTreeRoot, unsigned int ascii, qHead* codeBitList, 
                                                         unsigned int length)
{
   unsigned int* codeword ;
   
   codeword = (unsigned int*) 
               safe_malloc ( length * (sizeof(int)) ) ;
   build_codeword (huffmanTreeRoot, ascii, codeBitList, codeword, 0, -1) ;
}

/*-----------------------------------------------------------------------------
| Function: build_codeword
|     Auxiliary function to generate_codeword
|
| Algorithm:
|  Recursively traverses the huffman tree, storing the direction of its travel
|  (left or right) in an array 'codeword' until it hits the leaf node storing 
|  the required character.
|  The array elements of 'codeword', corresponds to the huffman code of the 
|  given character.
|  This array is passed to the helper function "generate_code_bit_list"
|  to build the huffman code as a singly linked list of individual bits.
|
| Parameters:
| -       hNode: Pointer to the current huffman node to traverse
| -       ascii: The ascii value of the character to encode
| - codeBitList: The head of the singly linked list to store the individual
|                huffman bits.
| -    codeword: The array of bits, indicating which edges have been 
|                traversed (0 means left, 1 means right). The final array
|                once the leaf with the ascii value is found, represents
|                the huffman code for that leaf.
| -        edge: The edge number of the path to the leaf node.
| -   direction: Which direction the function has traversed. Left = 0,
|                Right = 1. The special value -1 signifies that the 
|                function is traversing the root node.
*----------------------------------------------------------------------------*/
static void
build_codeword 
(huffNode* hNode, unsigned int ascii, qHead* codeBitList, 
            unsigned int* codeword, unsigned int edge, short int direction)
{
   if (hNode == NULL) {
      return ;
   }
   else {
      /* Check if the node is a root node, signified by direction = -1 */
      if (direction != -1) {
         codeword[edge] = direction ;
      }
      
      edge++ ;
      
      /* Found the leaf node with the needed character */
      if ((hNode->character == ascii) && (hNode->left == NULL) && 
                                                (hNode->right == NULL)) {
         generate_code_bit_list (codeword, edge, codeBitList) ;
      }
      else { /* Continue searching the tree for the character */
         build_codeword (hNode->left, ascii, codeBitList, codeword, edge, 0) ;
         build_codeword (hNode->right, ascii, codeBitList, codeword, edge, 1) ;
      }
   }
}

/*-----------------------------------------------------------------------------
| Function: generate_code_bit_list
|     Auxiliary function to build_codeword
|
| Algorithm:
|  Iterates through the array 'codeword', putting each value into
|  a node that is enqueued into the linked list, 'codeBitList'.
|
| Parameters:
| -       hNode: Pointer to the current huffman node to traverse
| -       ascii: The ascii value of the character to encode
| - codeBitList: The head of the singly linked list to store the individual
|                huffman bits.
| -    codeword: The array of bits, indicating which edges have been 
|                traversed (0 means left, 1 means right). The final array
|                once the leaf with the ascii value is found, represents
|                the huffman code for that leaf.
| -        edge: The edge number of the path to the lead node.
| -   direction: Which direction the function has traversed. Left = 0,
|                Right = 1. The special value -1 signifies that the 
|                function is traversing the root node.
|  
*----------------------------------------------------------------------------*/
static void
generate_code_bit_list 
(unsigned int* codeword, unsigned int edge, qHead* codeBitList)
{
   unsigned int i ;
   qNode*   codebitQueueNode ;
   
   for (i = 1; i < edge; i++) {
      codebitQueueNode = create_qnode (codeword[i]) ;
      enqueue_qnode (codeBitList, codebitQueueNode) ;
   }

}



/*-----------------------------------------------------------------------------
| Function: destroy_hufftree
|
| Description:
|  Frees all the memory used by the huffman tree.
|
| Algorithm:
|  Recursively traverse to the bottom of the tree and then start freeing the
|  nodes.
|
| Parameters:
|  - hNode: Pointer to the root of the huffman tree. 
*----------------------------------------------------------------------------*/
void
destroy_hufftree 
(huffNode* hNode) 
{
   if (hNode != NULL) {
      destroy_hufftree (hNode->left) ;
      destroy_hufftree (hNode->right) ;
      free (hNode) ;
   }      
}


/*=============================================================================
||                               DEBUG FUNCTIONS                             ||
=============================================================================*/


/*-----------------------------------------------------------------------------
| Function: print_paths
|
|  (Debug Function)
|  Prints all the paths in a huffman tree. 
|  The Result would look something like:
|   || freq: 27 || || freq: 10 || || freq: 5, char: 65 ||
|
|  where the first node is the root and the last node is the leaf.
*----------------------------------------------------------------------------*/
void
print_paths 
(huffNode* hNode)
{
   huffNode* path ;
   
   path = (huffNode*) calloc ( (hufftree_size (hNode)), (sizeof(hNode)) ) ;
   print_paths_prime (hNode, path, 0) ;
}

/*-----------------------------------------------------------------------------
| Function: print_paths_prime
|     Auxiliary function to print_paths
*----------------------------------------------------------------------------*/
static void
print_paths_prime 
(huffNode* hNode, huffNode* path, int pathLen)
{
   if (hNode == NULL) {
      return ;
   }
   else {
      path[pathLen] = (*hNode) ;
      pathLen++ ;
      
      if ((hNode->left == NULL) && (hNode->right == NULL)) {
         printArray (path, pathLen) ;
      }
      else {
         print_paths_prime ((hNode->left), path, pathLen) ;
         print_paths_prime ((hNode->right), path, pathLen) ;
      }
   }
}

/*-----------------------------------------------------------------------------
| Function: printArray
|     Auxiliary function to print_paths_prime
*----------------------------------------------------------------------------*/
static void
printArray 
(huffNode* path, unsigned int pathLen)
{
   unsigned int i ;
   
   for (i = 0; i < pathLen; i++) {
      print_huffnode (path[i]) ;
   }
   
   printf ("\n") ;
}



/*-----------------------------------------------------------------------------
| Function: print_huffnode
|
|  (Debug Function)
|  Prints a huffman node in the form: 
|   "|| freq: %3d ||"
|  Or for leaf nodes:
|   "|| freq: %3d  , char: %3d ||"
*----------------------------------------------------------------------------*/
void
print_huffnode 
(huffNode hNode)
{  
    printf ("  || ") ;
    printf ("freq: %3d", hNode.freq) ;
    
    if ((hNode.left == NULL) && (hNode.right == NULL))  {
      printf (" , char: %3d", hNode.character) ;
    }
    
    printf (" || ") ;
}



/*-----------------------------------------------------------------------------
| Function: print_hufftree
|
|  (Debug Function)
|  Prints all the nodes in the huffman tree from the 'bottom-up'.
*----------------------------------------------------------------------------*/
void
print_hufftree 
(huffNode* hNode) 
{
   if (hNode == NULL) {
      return ;
   }
   else {
      print_hufftree (hNode->left) ;
      print_hufftree (hNode->right) ;
      print_huffnode ((*hNode)) ;
   }
}



/*-----------------------------------------------------------------------------
| Function: hufftree_size
|
|  (Debug function)
|  Returns the total number of nodes in the given huffman tree.
*----------------------------------------------------------------------------*/
unsigned int
hufftree_size 
(huffNode* hNode)
{
   if (hNode == NULL) {
      return 0 ;
   }
   else {
      return ( hufftree_size(hNode->left) + 1 + hufftree_size(hNode->right) ) ;
   }
}

