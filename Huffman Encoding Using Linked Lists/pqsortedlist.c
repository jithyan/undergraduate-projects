/*=============================================================================
|  Author: Jithya Nanayakkara
|  Last Modified: 14/Oct/2011
|  Userid: vbjdn
+------------------------------------------------------------------------------
| Description: 
|   Functions and data structures for managing a minimum key priority queue 
|   used to store huffman nodes. 
|   This priority queue is implemented as a sorted linked list.
|   The priority queue's key is the frequency value of the huffman node.
|
| The data structures defined in pqsortedlist.h are:
|  pqNode: A node that stores a pointer to a huffman node and a pointer to the
|          next pqNode in the queue.
|  pqHead: Stores a pointer to the first node in the priority queue.
|
|
| The functions can:
|  - Allocate a new priority queue node
|  - Print the huffman nodes in the priority queue (for debugging)
|  - Find the length of the priority queue.
|  - Check if the priority queue is correctly sorted according to its key
|    (for debugging).
|  - Insert a new huffman node into the correct position in the queue 
|    (based on its frequency).
|  - Return the lowest keyed huffman node in the priority queue.
+============================================================================*/

#include "pqsortedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"


/*-----------------------------------------------------------------------------
| Function: create_node
|
| Description:
|  Allocates a new priority queue node, that stores the location of a huffman
|  node.
|
| Parameters:
|  - newHuffNode: A pointer to the address of the huffman node you wish to 
|                 insert into the priority queue.
|
| Returns: A pointer to the newly allocated pqNode.
*----------------------------------------------------------------------------*/
pqNode* 
create_node 
(huffNode** newHuffNode)
{
    pqNode* newNode ;
    
    newNode = safe_malloc (sizeof(pqNode)) ;
    newNode->hNode = (*newHuffNode) ;

    return newNode ;
}



/*-----------------------------------------------------------------------------
| Function: enqueue
|
| Description:
|  Inserts a qNode into a priority queue, its position in the queue is based
|  on the frequency value of the qNode's huffman node.
|
| Algorithm:
|  Traverse through the nodes in the priority queue, keeping track of the 
|  current node in the queue and the previous node during traversal.
|  If the queue is empty, traversal isn't done and the head is set to point
|  at the new node.
|
|  Otherwise traverse the queue until you either reach its end or the
|  frequency of the new node is lower than the current node.
|  Then set the new node to point to the current node (from the traversal)
|  and set the previous node (obtained from traversal) to point to the new node.
|
| Parameters:
|  - newNode: A pointer to the new node you want to insert.
|  - pqueue: The head of the priority queue you wish newNode to be inserted to.
*----------------------------------------------------------------------------*/
void
enqueue 
(pqHead* pqueue, pqNode* newNode)
{
   pqNode* current ;
   pqNode* previous ;
   
   for (previous = NULL, current = pqueue->next; 
         current != NULL;
            previous = current, current = current->next) {
               
      if (newNode->hNode->freq < current->hNode->freq) {
         break ;
      }
   }
   
   newNode->next = current ;
   
   /* Check if the priority queue is empty */
   if (previous != NULL) {
      previous->next = newNode ;
   }
   else {
      pqueue->next = newNode ;
   }
}
      


/*-----------------------------------------------------------------------------
| Function: dequeue
|
| Description:
|  Removes the node with the lowest frequency from the priority queue and returns
|  the pointer to the huffman node.
|  Since this is a sorted list priority queue, this is always the first
|  node in the list.
|
| Parameters:
|  - pqueue: The head of the priority queue you wish to dequeue.
|
| Returns: A pointer to the huffman node that was dequeued.
*----------------------------------------------------------------------------*/   
huffNode*
dequeue 
(pqHead* pqueue)
{
    if (pqueue->next == NULL) {
      fprintf (stderr, "Error: Dequeueing from an empty queue\n") ;
      exit(EXIT_FAILURE) ;
    }
    else {
      pqNode*  tempNode ;
      huffNode*  removedHuffNode ;

      /* Get the second node in the list */
      tempNode = pqueue->next->next ;
      
      removedHuffNode = pqueue->next->hNode ;
      free (pqueue->next) ;
      pqueue->next = tempNode ;

      return removedHuffNode ;
   }
}



/*-----------------------------------------------------------------------------
| Function: length
|
| Description:
|  Returns the number of nodes stored in the priority queue.
|
| Parameters:
|  - pqueue: The head of the priority queue you wish to dequeue.
|
| Returns: The length of the priority queue.
*----------------------------------------------------------------------------*/  
unsigned int 
length
(pqHead pqueue)
{
   unsigned int count   = 1 ;
   pqNode*      current = pqueue.next ;
   
   while (current != NULL) {
      current = current->next ;
      count++ ;
   }
   
   return count ;
}


/*=============================================================================
||                               DEBUG FUNCTIONS                             ||
=============================================================================*/


/*-----------------------------------------------------------------------------
| Function: print_list
|
|  (Debug Function)
|  Prints huffman nodes stored in the priority queue.
|  See "huffmantree.h" for more information on the function 'print_huffnode'
*----------------------------------------------------------------------------*/
void 
print_list 
(pqHead pqueue)
{
   unsigned int count   = 1 ;
   pqNode*      current = pqueue.next ;
   
   printf ("Printing Priority List...\n\n") ;

   while (current != NULL) {
      printf ("   Node: %3d\n", count) ;
      print_huffnode (*(current->hNode)) ;
      current = current->next         ;
      count++ ;
   }

   printf ("END OF LIST\n") ;
}



/*-----------------------------------------------------------------------------
| Function: checkSorted
|
|  (Debug Function)
|  Checks if all the nodes are sorted in ascending order of the frequency
|  values of their huffman nodes.
*----------------------------------------------------------------------------*/
bool
checkSorted 
(pqHead pqueue)
{
   pqNode* current = pqueue.next ;
   
   while ( (current != NULL) && (current->next != NULL) ) {
      if (current->hNode->freq > current->next->hNode->freq) {
         return false ;
      }
      current = current->next ;
   }
   
   return true ;      
}

