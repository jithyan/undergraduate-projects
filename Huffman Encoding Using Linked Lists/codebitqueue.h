/*=============================================================================
|  Author: Jithya Nanayakkara
|  Last Modified: 14/Oct/2011
|  Userid: vbjdn
+============================================================================*/

typedef struct queuenode qNode ;

struct queuenode {
   unsigned short int bit ;
   qNode*   next ;
} ;

typedef struct {
   qNode* first ;
   qNode* last ;
   unsigned int length ;
} qHead ;



void 
   enqueue_qnode (qHead* queue, qNode* newNode) ;
   
qNode* 
   create_qnode (unsigned short int bit) ;
   
void 
   print_queue (qHead queue) ;

unsigned short int 
   dequeue_qnode (qHead* queue) ;

void 
   destroy_queue (qHead* queue) ;

