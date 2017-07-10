/*=============================================================================
|  Author: Jithya Nanayakkara
|  Last Modified: 14/Oct/2011
|  Userid: vbjdn
+------------------------------------------------------------------------------
|Description: 
| The purpose is to provide a custom data structure to hold huffman codewords
| as a linked list of individual bits, in order to more efficiently store 
| the variable length huffman codewords.
| A queue is used for managing the bits in this linked list. Items are inserted
| to the back of the queue and removed from the front.
|
| The structures defined in codebitqueue.h are:
| - qNode: The nodes in the linked list that hold each binary bit 
|          in a huffman codeword
| - qHead: Points to the first & last nodes in the list and maintains the
|          length of the list.
|
| The functions can:
|  - Allocate a new qNode
|  - Enqueue a new qNode to a given queue
|  - Dequeue a qNode from a given queue
|  - Deallocate all the memory used in a queue.
|  - Print the bits in a queue
+============================================================================*/


#include "codebitqueue.h"
#include <stdio.h>
#include "util.h"


/*-----------------------------------------------------------------------------
| Function: create_qnode
|
| Description:
|  Allocates a new qNode and set its bit field to the given bit argument.
|
| Parameters:
|  - bit: A 0 or 1.
|
| Returns: A pointer to the new qNode.
*----------------------------------------------------------------------------*/
qNode* 
create_qnode 
(unsigned short int bit)
{
   qNode* newNode ;
   
   newNode = (qNode*) safe_malloc (sizeof (qNode)) ;
   newNode->bit = bit ;
   newNode->next = NULL ;
   
   return newNode ;
}   



/*-----------------------------------------------------------------------------
| Function: enqueue_qnode 
|
| Description:
|  Adds a given qNode to the end of a given queue.
|
| Algorithm:
|  If the queue is empty, then set the first and last fields of the qHead to
|  point to the given node.
|  Otherwise set the next field of the last node of the queue to point to the
|  new node, then put the new node to the back of the queue.
|  Finally make sure the new node is set to NULL and increment the length of
|  the queue.
|
| Parameters:
|  -   queue: A Pointer to the head of the queue.
|  - newNode: Pointer to the node to enqueue.
*----------------------------------------------------------------------------*/
void 
enqueue_qnode 
(qHead* queue, qNode* newNode)
{
   if (queue->first == NULL) {
      queue->first = newNode ;
      queue->last = newNode ;
   }
   else {     
      queue->last->next = newNode ;
      queue->last = newNode ;
   }
   
   newNode->next = NULL ;
   queue->length = (queue->length) + 1 ;
}



/*-----------------------------------------------------------------------------
| Function: dequeue_qnode
|
| Description:
|  Removes the first node from a queue and returns the binary bit it held.
|
| Algorithm:
|  Check if the queue being dequeued is empty. If so then terminate.
|  Set the 'first' field of the queue head to point to the second node in the
|  queue, deallocate the former first node of the queue and return the codeBit
|  it was pointing to.
|
| Parameters:
|  - queue: Pointer to the head of the queue.
|
| Returns: Returns a 1 or 0.
*----------------------------------------------------------------------------*/
unsigned short int
dequeue_qnode 
(qHead* queue)
{
   if (queue->first == NULL) {
      fprintf (stderr, "Error: Dequeueing from an empty queue\n") ;
      exit(EXIT_FAILURE) ;
   }
   else {
      qNode*   tempNode ;
      unsigned short int removedBit ;
      
      tempNode = queue->first ;
      queue->first = tempNode->next ;
      queue->length = (queue->length) - 1 ;
      
      if (queue->first == NULL) {
         queue->last = NULL ;
      }
      
      removedBit = tempNode->bit ;
      free (tempNode) ;
      
      return removedBit ;
   }
}



/*-----------------------------------------------------------------------------
| Function: destroy_queue 
|
| Description:
|  Deallocates all the queue nodes stored in the queue.
|
| Parameters:
|  - queue: Pointer to the head of the queue.
*----------------------------------------------------------------------------*/
void
destroy_queue 
(qHead* queue)
{
   while (queue->first != NULL) {
      dequeue_qnode (queue) ;
   }
}



/*-----------------------------------------------------------------------------
| Function: print_queue
|
| Description:
|  Prints all the bits held in the given queue.
|
| Parameters:
|  - queue: Pointer to the head of the queue.
*----------------------------------------------------------------------------*/
void 
print_queue 
(qHead queue)
{
   qNode* current_node ;
      
   current_node = queue.first ;
     
   while (current_node != NULL) {
      printf ("%d", (current_node->bit)) ;
      current_node = current_node->next ;
   }
   
   printf ("\n") ;
}

