Name: Jithya Nanayakkara
Userid: vbjdn

The data structures used:
   - A huffman tree defined in huffmantree.c
   - A priority queue sorted list, defined in pqsortedlist.c
   - A priority queue binary search tree, defined in pqbst.c
   - A linked list that stores the individual huffman bits, stored in
     codebitqueue.c


When the user wishes to encode or decode a file, the -B option should be
included at the command line. This would set the switch 'use_bst' to 1.

In the event that the user wishes to encode a file, the 'encode' function is
called which has the following parameters:

  -       inp_file: The file to read in.
  - char_frequency: The frequency of the ascii values that are read.
  -   huffmanCodes: Where the huffman codewords for the ascii values read in
                    are stored (as a linked list of bits).
  -       roothuff: Pointer to the root of the huffman tree.
  -      outp_file: The file to write the encoding to.
  -        use_bst: User has defined to use a BST priority queue or not.
  
  char_frequency is an array size 256. 
  huffmanCodes is an array of linked lists.  The linked list is a list of 
  invdividual bits which form a huffman code. An alternate approach would 
  have been to use a 2D array which would be inefficient in terms of storage 
  as the array size would have to be fixed to the maximum length of the 
  codeword, though accessing the bits would have been faster.
  
  In the function encode, the first thing it does is to read the input file and
  record the frequencies of all the ascii values that occur in the file.
  This would take Theta(n) where n is the number of characters in the file.
  The frequencies are stored in char_frequency. 
  Afterwards, the number of unique  ascii values is counted by looping through
  char_frequency. Such a loop has O(256) complexity.
  
  If the file is empty, the function simply encodes an output file with
  only the int 0 written to it.
  
  Otherwise, the function "get_huffman_codewords" is called, 
  which does the following:
  > Build a priority queue depending on the option the user has selected.
  > Build a huffman tree using that priority queue
  > Generate a linked list of codewords for the required ascii values
  
  The above three processes are done using sub-functions.
  There are 2 different functions for building the priority queue
  and 2 different functions for constructing the huffman tree, 
  each using a specific data structure for the priority queue 
  (either the Binary Search Tree or Sorted Linked List).
  Which function is called depends on whether the user has entered the 
  -B option or not.
  
  Enqueueing elements into a sorted link list would have a complexity of O(n),
  where n is the number of unique characters read from the input file.
  Dequeueing elements from a sorted link list would have a complexity 
  of Theta(1).
  
  Inserting a node into BST priority queue has a complexity of O(h), 
  where h is the height of the tree.
  Removing the lowest key node from a BST involves first finding the node with 
  the smallest key and deleting it from the node. 
  Finding the smallest key takes time O(h) and deleting takes O(h) where h 
  is the height of the tree. However the height of the tree depends on how
  balanced the tree is when built. 
  For example, if n items  are inserted into a tree in increasing order, 
  the tree will be a chain with height n-1, where n is the number of unique 
  ascii values read. 
  So the many delete and insert operations when creating the huffman tree would
  change the structure of the BST, making its performance variable.
  
  
  After the huffman tree has been built , the final function called within 
  get_huffman_codewords is "generate_codewords", which traverses the huffman 
  tree to build the codewords. 
  These codewords are stored in the array of linked lists, 'huffmanCodes'.
  
  Afterwards, in the function 'encode', the function "write_compressed_file" is
  called.  This function writes the header into the output file, rereads the 
  input file and for every ascii value it reads, it puts the appropriate 
  huffman bits by calling the function write_huffman_bits. This function simply
  traverses the linked list of bits that make up the huffman codewords 
  for a given ascii character ands puts each bit into the output file.
  
  The last operation conducted by encode is to free all the memory used by 
  the huffman tree. 
  
  
  If the user wishes to decode a file, the function decode is called which has
  the same parameters as encode. It calls the function get_header_frequencies 
  to extract the character frequencies and the total number of characters to 
  decode. 
  Then the function 'get_huffman_codewords' is re-used to generate the
  huffman codewords. 
  
  Afterwards, in a loop, the function getbit is called to read the bits from 
  the input file.  Each bit read is used to traverse the huffman tree by 
  using the function traverse_tree.
  If traverse_tree returns a leaf node, the ascii character held by that node 
  is written  to the output file. Then the counter for the total number of 
  characters to decode  is decremented, and the loop terminates once that 
  counter reaches 0.
  
  Once that is finished, decode frees all the memory used by the huffman tree.
  Once encode or decode is finished, the program will print out the character 
  frequencies and huffman codewords if the user has specified it 
  (the huffman codewords array is defined in the main function, and its 
  reference is modified by the functions called by encode or decode).

  Finally, memory used by char_frequency and the linked lists in 
  huffmanCodes are freed.

  
  For more detailed information about the functions used, please read the 
  comments in the C Files.
  
  
