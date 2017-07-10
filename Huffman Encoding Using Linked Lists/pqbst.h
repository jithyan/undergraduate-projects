/*=============================================================================
|  Author: Jithya Nanayakkara
|  userid: vbjdn
|  Last Modified: 14/11/2011
+=============================================================================*/
#include "huffmantree.h"

typedef struct bst_node bstNode ;

struct bst_node {
    bstNode*  left   ;
    bstNode*  right  ;
    bstNode*  parent ;
    huffNode* hNode  ;
} ;


bstNode* 
create_bst_node (huffNode** newHuffNode, bstNode* left, bstNode* right, bstNode* parent) ;

void 
insert_bstnode (bstNode** root, bstNode* newNode) ;

huffNode* 
remove_priority_bstnode (bstNode** root) ;

void
inorder_tree_walk (bstNode* bstN) ;

unsigned int
bst_size 
(bstNode* bstN) ;

