#ifndef HUFFMANTREE_H_
#define HUFFMANTREE_H_

#include "codebitqueue.h"


typedef struct anode huffNode ;

struct anode {
   unsigned int freq ;
   unsigned int character ;
   huffNode*    left ;
   huffNode*    right ;
} ;


unsigned int 
   hufftree_size (huffNode* hNode) ;
   
void 
   print_huffnode (huffNode hNode) ;
   
void 
   print_hufftree (huffNode* huffmanRoot) ;

huffNode* 
   create_huffnode (unsigned int freq, unsigned int character, 
                                          huffNode* left, huffNode* right) ;

void 
   print_paths (huffNode* hNode) ;

void 
   generate_codeword (huffNode* huffmanTreeRoot, unsigned int ascii, 
                                  qHead* codeBitList, unsigned int length) ;

unsigned int 
   get_max_depth (huffNode* hNode) ;

void 
   destroy_hufftree (huffNode* hNode) ;
   
huffNode* 
   traverse_tree (huffNode* hNode, unsigned short int bit) ;
   
#endif

