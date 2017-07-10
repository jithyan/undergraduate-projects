/*=============================================================================
|  Author: Jithya Nanayakkara
|  Last Modified: 11/Oct/2011
|  Userid: vbjdn
+============================================================================*/

#include <stdbool.h>
#include "huffmantree.h"


typedef struct node pqNode ;

struct node {
    pqNode*  next ;
    huffNode* hNode ;
} ;

typedef struct {
    pqNode* next ;
} pqHead ;



pqNode* 
   create_node (huffNode** newHuffNode) ;

unsigned int 
   length (pqHead pqueue) ;

void 
   print_list (pqHead pqueue) ;

void 
   enqueue (pqHead* pqueue, pqNode* newNode) ;


bool 
   checkSorted (pqHead pqueue) ;
 
huffNode* 
   dequeue (pqHead* pqueue) ;

