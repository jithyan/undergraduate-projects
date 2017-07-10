
/*
 COMP20003 Algorithms and Data Structures

*/



/**
Name: Jithya Nanayakkara
login id: vbjdn

Attempted Questions for Bonus marks: Yes
                                     Implemented only a binary search tree.
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include "util.h"
#include "bit_io.h"
#include "pqsortedlist.h"
#include "pqbst.h"

#define MAX_CHAR 256
#define MAX_FILENAME 75



static void 
  encode (FILE *inp_file, int *char_frequency, qHead huffmanCodes[], 
                                          FILE* outp_file, int use_bst) ;
                                          
static void 
   decode (FILE* inp_file, int* char_frequency, qHead huffmanCodes[], 
                                                 FILE* outp_file, int use_bst) ;
   
static void 
   usage(void);
   
static void
   print_freq_table (int *char_frequency) ;
   
static void
   build_priority_queue (pqHead* pqueue, int* char_frequency) ;
   
static huffNode* 
   create_huffmantree (pqHead* pqueue) ;
   
static void 
   print_codeword_table (qHead huffmanCodes[], int* char_frequency) ;
   
static void 
   write_compressed_file (qHead huffmanCodes[], int* char_frequency, 
                                          FILE* inp_file, FILE* outp_file) ;
                                          
static void 
   write_huffman_bits (FILE* outp_file, qHead* huffmanCodeBits) ;

static void
   check_read_error (int read_error, const char* error_message) ;

static void
   get_header_frequencies (FILE* inp_file, int* char_frequency, 
                                                      qHead huffmanCodes[]) ;
   
static void
   get_huffman_codewords (int* char_frequency, qHead huffmanCodes[],
                                            huffNode** roothuff, int use_bst) ;                                                     
   
static huffNode* 
   create_huffmantree_with_bst (bstNode** bstroot) ;

static void 
   build_bst_priority_queue (bstNode** bstroot, int* char_frequency) ;
   
                                                         
/* 
 * main()
 *
 *
 * Parameters:
 *      argc    - command line argument count
 *      argv    - array of command line argument
 *
 */
int
main (int argc, char **argv)
{
  FILE *inp_file = NULL;
  FILE *outp_file = NULL;
  char inputfile[MAX_FILENAME];
  char outputfile[MAX_FILENAME];

  /* Stores the ascii values read in and the frequency of their occurence. 
   * The index of the array corresponds to the ascii value. */
  int *char_frequency ;
  
  /* Stores an array of linked lists of bits that makeup a huffman codeword, 
   * one for each possible ascii value.
   * The additional space (MAX_CHAR + 1) is to store:
   * (a) When encoding - the number of unique characters read in.
   * (b) When decoding - the total number of characters to decode */
  qHead  huffmanCodes[(MAX_CHAR + 1)] ;

  int i;
  int c;

  /* Specify all the flags needed for option handling */

  /* Note that file_process_procedure 
   * = 1 (means encoding) or 2 (means decoding)  
   */
  int file_process_procedure = 0;
  int print_frequency = 0;  
  int print_codeword = 0;
  int outputgiven = 0;
  int use_bst = 0 ;


  /* Check whether command line has at least three arguments */
  if (argc < 3)
  {
    fprintf(stderr, "Not enough arguments \n"); 
    usage();
  }  

  /* Process command line options */
  while ((c = getopt (argc, argv, "edBfco:")) != -1)
  {
    switch (c)
    {
        case 'e': 
         /* Print error message if multiple methods are defined in option
          * i.e. both -e and -d are given in the option
          */
           if (file_process_procedure)
           {
                fprintf (stderr, " Only one method is allowed \n");
                usage();
           }
           file_process_procedure = 1;
           break;

        case 'd': 
         /* Print error message if multiple methods are defined in option
          * i.e. both -e and -d are given in the option
          */
           if (file_process_procedure)
           {
                fprintf (stderr, " Only one method is allowed \n");
                usage();
           }
           file_process_procedure = 2;
           break;
        
        case 'f': 
           print_frequency = 1;
           break;

        case 'c': 
           print_codeword = 1;
           break; 
           
        case 'B':
           use_bst = 1 ;
           break ;
    
        case 'o': /* output is given (takes a string argument) */
           outputgiven = 1; 
           if (sscanf(optarg, "%s", outputfile) != 1)
           {
               fprintf (stderr, "the -o option needs a string argument\n");
               usage();
           }
           break;
        
        case '?': /* unrecognized option */
           usage();
           break;

        default: /* shouldn't happen */
           usage();
           break;
    }
  }

  /* Ensure that one of both methods (i.e. encoding or decoding) has been
   * specified by users in the option
   */ 
  if (!file_process_procedure)
  {  
    fprintf(stderr, "Method used (either -d or -e) has to be specified \n"); 
    usage();
  }

  /* Ensure that the number of arguments entered are exact */
  if (optind < (argc-1))
  {
    fprintf(stderr, "Too many arguments\n");
    usage();
  }

  else if (optind > (argc-1))
  {
    fprintf(stderr, "Input file must be provided\n");
    usage();
  }

  /* Ensure that the input_file has been given in the command line 
   * and in string format
   */
  if (sscanf(argv[optind],"%s", inputfile) != 1)
  {
    fprintf(stderr, "the input file must be string argument\n");
    exit (EXIT_FAILURE);
  }
 
  /* Open the provided output file if given */

  if (outputgiven)    
  { 
        outp_file = safe_fopen (outputfile, "w");
  } else
  {
 /* Use default output file as inputfile.out */
      strcpy(outputfile,inputfile);
      strcat(outputfile,".out");
      outp_file = safe_fopen (outputfile, "w");
  }

  /* Open the provided input file */
  inp_file = safe_fopen (inputfile,"r") ;

  char_frequency = (int *) safe_malloc (sizeof (int) * (MAX_CHAR + 1));
  
  for (i = 0; i < MAX_CHAR; i++)
  {
    char_frequency[i] = 0;
  }

  if (file_process_procedure == 1) {
    encode (inp_file, char_frequency, huffmanCodes, outp_file, use_bst) ;      
  }
  else if (file_process_procedure == 2) {
    decode (inp_file, char_frequency, huffmanCodes, outp_file, use_bst) ;
  }
 

  /*Print the frequencies if -f option is specified*/ 
  if (print_frequency) {
    print_freq_table (char_frequency) ;
  }

  /*Print Codewords if -c option is specified*/
  if (print_codeword) {
    print_codeword_table (huffmanCodes, char_frequency) ;
  }

  /* close input file */
  fclose (inp_file) ;
  fclose (outp_file) ;
   
  /* Free all the memory used */
  free (char_frequency);
  
  if (huffmanCodes[MAX_CHAR].length != 0) {
      /* Not an empty file, therefore memory to free */
      for (i = 0; i < MAX_CHAR; i++) {
         if (huffmanCodes[i].length > 0) {
            destroy_queue (&huffmanCodes[i]) ;
         }
      }
  }

  return 0;
}



/*-----------------------------------------------------------------------------
| Function: encode
|
| Description:
|  Huffman encodes an input file to an output file.
|
| Algorithm:
|  Read and store the ascii characters and their frequencies from the file.
|  If the file is empty, write the output file with 0 as its header.
|  Otherwise, use the character frequencies to build the huffman tree. 
|  The input file then needs to be read in again from the start, so the 
|  function rewind is called before writing the final huffman encoding 
|  to the specified output file. 
|
| Parameters:
|  -       inp_file: The file to read in.
|  - char_frequency: The frequency of the ascii values that are read.
|  -   huffmanCodes: Where the huffman codewords for the ascii values read in
|                    are stored (as a linked list of bits).
|  -       roothuff: Pointer to the root of the huffman tree.
|  -      outp_file: The file to write the encoding to.
|  -        use_bst: User has defined to use a BST priority queue or not.
*----------------------------------------------------------------------------*/
static void 
encode 
(FILE *inp_file, int *char_frequency, qHead huffmanCodes[], FILE* outp_file,
   int use_bst)
{
  /* Counter for the number of unique characters read in from the input file */
  unsigned int count = 0;
  int ascii = 0;
  huffNode* roothuff  = NULL;

  while ((ascii = fgetc(inp_file)) != EOF) {
     char_frequency[ascii] += 1 ; 
  }

  for (ascii = 0; ascii < MAX_CHAR; ascii++) {
     if (char_frequency[ascii] > 0) {
        count++ ;
     }
  }
  
  /* If the file is empty, write the empty output file with 0 as the header. */
  if (count == 0) {
    fwrite (&count, sizeof(int), 1, outp_file) ;
    return ;
  } 
  
  get_huffman_codewords (char_frequency, huffmanCodes, &roothuff, use_bst) ;
  
  /* Store the number of unique characters in the final element of huffmanCodes */
  huffmanCodes[MAX_CHAR].length = count ;   
  
  /* Write the output */
  rewind (inp_file) ;   
  write_compressed_file (huffmanCodes, char_frequency, inp_file, outp_file) ;
  destroy_hufftree (roothuff) ;
}



/*-----------------------------------------------------------------------------
| Function: decode
|
| Description:
|  Decompresses a huffman encoded file to an output file.
|
| Algorithm:
|  Extract the character frequencies and total number of characters
|  to decode from the header.
|  If the encoded file is an encoding of an empty file, then terminate the
|  function (an empty output file has been automatically created before).
|
|  Otherwise use the extracted character frequencies to build the huffman tree.|
|  Then read each bit from the input file (starting after the header).
|  Use the bit to traverse the huffman tree. Continue reading bits until you
|  hit the leaf node of the huffman tree - at that point you write the character
|  value stored in the node to the output file and decrement the count of 
|  the total number of character to decode. Keep writing until that count reaches
|  0.
|  Continue reading the bits from the file until you reach the end.
|
| Parameters:
|  -       inp_file: The encoded file to decode.
|  - char_frequency: The frequency of the ascii values read in.
|  -   huffmanCodes: Where the huffman codewords for the ascii values read in
|                    are stored (as a linked list of bits).
|  -      outp_file: The file to write the encoding to.
|  -        use_bst: User has defined to use a BST priority queue or not.
*----------------------------------------------------------------------------*/
static void
decode 
(FILE* inp_file, int* char_frequency, qHead huffmanCodes[], FILE* outp_file,
   int use_bst)
{

   int bit = 0;
   huffNode* tempHuffNode = NULL ;
   huffNode* roothuff = NULL ;
   
   get_header_frequencies (inp_file, char_frequency, huffmanCodes) ;
   
   /* Check if file is empty */
   if (huffmanCodes[MAX_CHAR].length == 0) return ;
      
   get_huffman_codewords (char_frequency, huffmanCodes, &roothuff, use_bst) ;
   tempHuffNode = roothuff ;

   /* huffmanCodes[MAX_CHAR] stores the total number of characters to decode.
    * Start decoding the input file and write its output */
   while ( (huffmanCodes[MAX_CHAR].length) > 0 ) {

      bit = getbit (inp_file) ;

      if (bit == EOF) {
        fprintf (stderr, "Input file ends prematurely!") ;
        exit (EXIT_FAILURE) ;
      }

      tempHuffNode = traverse_tree (tempHuffNode, bit) ;
      
      if ( (tempHuffNode->left == NULL) && (tempHuffNode->right == NULL) ) {
         fputc ((tempHuffNode->character), outp_file) ;
         tempHuffNode = roothuff ;
         (huffmanCodes[MAX_CHAR].length)-- ;
      }
   }

   destroy_hufftree (roothuff) ;
}



/*-----------------------------------------------------------------------------
| Function: get_huffman_codewords 
|
| Description:
|  Generates the huffman codewords for the given ascii values.
|  The codewords are a linked list of individual bits.
|
| Algorithm:
|  Build a priority queue of individual huffman nodes based on the
|  given characters and their frequencies. The data structure of the priority
|  queue is determined by the use_bst option specified by the user: 1 means
|  use a BST priority queue, otherwise use the default, a sorted linked list.
|  
|  Then use that priority queue to construct a huffman tree.
|  Finally traverse the huffman tree to build the codeword for each
|  ascii character read in.
|
| Parameters:
|  - char_frequency: The frequency of the ascii values read in.
|  -   huffmanCodes: Where the huffman codewords for the ascii values read in
|                    are stored (as a linked list of bits).
|  -       roothuff: Double pointer to the root of the huffman tree.
|  -        use_bst: User has defined to use a BST priority queue or not.
*----------------------------------------------------------------------------*/
static void
get_huffman_codewords 
(int* char_frequency, qHead huffmanCodes[], huffNode** roothuff, int use_bst)
{
  bstNode* bstroot ;
  pqHead pqueue ;

  /* The maximum possible length of the huffman codeword */
  int maxCodeLength ;
  int ascii ;
  
  
  /*Construct huffman tree, based on the data structure specified */
  if (use_bst == 1) {
      bstroot = NULL ;
      build_bst_priority_queue  (&bstroot, char_frequency) ;
      *roothuff = create_huffmantree_with_bst (&bstroot) ;
  }
  else {
      pqueue.next = NULL ;
      build_priority_queue (&pqueue, char_frequency) ;
      *roothuff = create_huffmantree (&pqueue) ;
  }

  maxCodeLength = get_max_depth (*roothuff) ;

  /* Build the codeword for each ascii value by traversing the huffman tree*/
  for (ascii = 0; ascii < MAX_CHAR; ascii++) {
      huffmanCodes[ascii].first = NULL ;
      huffmanCodes[ascii].last = NULL ;
      huffmanCodes[ascii].length = 0 ;
      
      if (char_frequency[ascii] > 0) {
         generate_codeword 
               (*roothuff, ascii, &(huffmanCodes[ascii]), maxCodeLength) ;
      }
  }
}



/*-----------------------------------------------------------------------------
| Function: build_priority_queue
|
| Description:
|  Builds a priority queue of huffman nodes, sorted in ascending order
|  according to their frequency.
|
| Algorithm:
|  Create a new huffman leaf node for each ascii value.
|  Encapsulate that node in a priority queue node.
|  Enqueue that node in the priority queue.
|
| Parameters:
|  -         pqueue: Pointer to the head of the priority queue.
|  - char_frequency: The frequency of the ascii values read in.
*----------------------------------------------------------------------------*/
static void 
build_priority_queue 
(pqHead* pqueue, int* char_frequency)
{
   unsigned int i = 0 ;
   pqNode* newNode ;
   huffNode* newHuffNode ;
   
   for (i = 0; i < MAX_CHAR; i++) {
      if (char_frequency[i] > 0) {
         newHuffNode = create_huffnode (char_frequency[i], i, NULL, NULL) ;
         newNode = create_node (&newHuffNode) ;
         enqueue (pqueue, newNode) ;
      }
   }
}



/*-----------------------------------------------------------------------------
| Function: get_header_frequencies 
|
| Description:
|  Reads the header of the input file, to extract:
|   - The total number of encoded characters in the input file.
|   - The frequencies of each unique ascii character.
|
| Algorithm:
|  The header has the format:
|  N C1 F1 ... Ci Fi
|     where N which is the unique number of characters encoded in the file.
|           Ci is the character that occurs in the file, stored as a char.
|           Fi is the frequency of that character, stored as an int.
|
| First extract N. Check for read errors.
| N is used to determine the number of characters in the header to read
|  (in other words, where the header ends).
| Keep extracting the character and frequency values while decrementing
| the extracted value of N. Also keep a count of the total number
| of characters to decode (by adding the frequencies of each character).
| Stop when N reaches 0.
|  
| Parameters:
|  -       inp_file: The encoded file to decode.
|  - char_frequency: The frequency of the ascii values read in.
|  -              N: The address of an int that is to hold the total number
|                    of characters to decode in the input file.
*----------------------------------------------------------------------------*/
static void 
get_header_frequencies 
(FILE* inp_file, int* char_frequency, qHead huffmanCodes[])
{
  unsigned int ascii = 0 ;
  unsigned int frequency = 0 ;
  unsigned int error = 0 ;  
  unsigned int totalNumOfChars = 0 ;
  unsigned int N = 0;
  
  /* Read the number unique of characters */
  fread (&N, sizeof(int), 1, inp_file) ;

  /* Read the characters and their frequencies in the header */
  while (N > 0) {
     ascii = fgetc (inp_file) ;
     error = fread (&frequency, sizeof (int), 1, inp_file) ;
     check_read_error 
         (error, "Error: Wrong/Corrupt file or incorrect header format.\n") ;
         
     char_frequency[ascii] = frequency ;
     N-- ;
     totalNumOfChars += frequency ;
  }
  
  /* Store the total number of characters to decode */
  huffmanCodes[MAX_CHAR].length = totalNumOfChars ;
}



/*-----------------------------------------------------------------------------
| Function: check_read_error 
|
| Description:
|  Determine if a read error has occurred, and the error message that should
|  be output if an error occurs.
|  A read error has occurred if a read operation returns 0 (no characters read)
|  or EOF when it should not have.
|
| Parameters:
|  -    read_error: The error code to validate.
|  - error_message: What the error message should be.
*----------------------------------------------------------------------------*/
static void 
check_read_error 
(int read_error, const char* error_message) 
{
  if ((read_error == 0) || (read_error == EOF)) {
    fprintf (stderr, error_message) ;
    exit(EXIT_FAILURE) ;
  }
}   



/*-----------------------------------------------------------------------------
| Function: create_huffmantree
|
| Description:
|  Constructs a huffman tree from a priority queue of huffman nodes.
|  The priority queue data structure is the default - a sorted link list.
|
| Algorithm:
|  Dequeues the two lowest frequency huffman nodes from the priority queue.
|  Creates a new huffman node that points to these two dequeued nodes.
|  Sets the frequency of the new huffman node as the sum frequencies of the two
|  previously dequeued nodes.
|  Enqueues the new huffman node into the priority queue.
|  This is repeated until there's only one huffman node in the priority queue.
|  This final node is dequeued and returned.
|
|  NOTE: The function for dequeueing a pqNode frees the memory used by that
|        node. See pqsortedlist.c for more details.
|
| Parameters:
|  - pqueue: Pointer to the head of the priority queue.
| 
| Returns: Pointer to the root of the huffman tree.
*----------------------------------------------------------------------------*/
static huffNode* 
create_huffmantree 
(pqHead* pqueue)
{
   unsigned int n = length ((*pqueue)) ;
   unsigned int i = 1 ;
   huffNode* newHuffNode = NULL ;
   huffNode* x = NULL ;
   huffNode* y = NULL ;
   pqNode* newNode = NULL ;
   
   for (i = 1; i < (n - 1); i++) {
      x = dequeue (pqueue) ;
      y = dequeue (pqueue) ;
      
      newHuffNode = create_huffnode ((x->freq + y->freq), NULL, x, y) ;
      newNode = create_node (&newHuffNode) ;
      enqueue (pqueue, newNode) ;
   }

   return (dequeue (pqueue));
}



/*-----------------------------------------------------------------------------
| Function: print_codeword_table 
|
| Description:
|  Prints the huffman codewords for the ascii values read in from the
|  input file. Its format is - <ascii>: <string of bits>.
|  <ascii> is formated to 3 digits.
|  Example:
|   32: 101
|
| Parameters:
|  -   huffmanCodes: Huffman codewords for the ascii values read in,
|                    stored as an array of linked lists of bits.
|  - char_frequency: The frequency of the ascii values read in.
*----------------------------------------------------------------------------*/
static void 
print_codeword_table 
(qHead huffmanCodes[], int* char_frequency)
{
  unsigned int ascii = 0 ;
   
  printf ("Codewords: \n");
   
  for (ascii = 0; ascii < MAX_CHAR; ascii++) {
      if (char_frequency[ascii] > 0) {
         printf ("%3d: ", ascii) ;
         print_queue (huffmanCodes[ascii]) ;
      }
  }
}



/*-----------------------------------------------------------------------------
| Function: write_compressed_file
|
| Description:
|  Writes the huffman code to the output file, based on the input from the 
|  input file. It also writes some header information for use when 
|  decoding the file.
|
| Algorithm:
|  First write the header to the output file. It's format is:
|  N C1 F1 ... Ci Fi
|     where N which is the number of unique characters encoded in the file.
|           Ci is the character that occurs in the file
|           Fi is the frequency of that character
|
|  Now read the input file, and for every ascii character read in,
|  call the write_huffman_bits function, which writes the 
|  huffman encoding of that ascii character to the file.
|
| Parameters:
|  -   huffmanCodes: Huffman codewords for the ascii values read in,
|                    stored as an array of linked lists of bits.
|  - char_frequency: The frequency of the ascii values read in.
|  -       inp_file: The file to read in.
|  -      outp_file: The file to write to.
*----------------------------------------------------------------------------*/
static void
write_compressed_file 
(qHead huffmanCodes[], int* char_frequency, FILE* inp_file, FILE* outp_file)
{
  int ascii = 0;
  unsigned int N = 0;
  unsigned int F = 0;
  
  /* huffmanCodes[MAX_CHAR].length stores the number unique of characters read 
   * from the input file. */
  N = huffmanCodes[MAX_CHAR].length ;
  fwrite (&N, (sizeof (int)), 1, outp_file) ;
  
  for (ascii = 0; ascii < MAX_CHAR; ascii++) {
      if (char_frequency[ascii] > 0) {
         F = (char_frequency[ascii]) ;
         fputc (ascii, outp_file) ;
         fwrite (&F, (sizeof (int)), 1, outp_file) ;
      }
  }
  
  /*Write Compressed Output */
  while ((ascii = fgetc(inp_file)) != EOF)  {
     write_huffman_bits (outp_file, &(huffmanCodes[ascii])) ;
  }

  putbit (outp_file, FLUSH_BYTE) ;
}



/*-----------------------------------------------------------------------------
| Function: write_huffman_bits 
|
| Description:
|  Writes the bits of a huffman codeword to an output file.
|
| Algorithm:
|  Traverse the linked list of bits, writing each node's bit to the output file.
|
| Parameters:
|  -   huffmanCodeBits: The linked list of bits that make up the huffman
|                       codeword.
|  -         outp_file: The file to write to.
*----------------------------------------------------------------------------*/
static void 
write_huffman_bits
(FILE* outp_file, qHead* huffmanCodeBits)
{
   qNode* current_node ;
   current_node = huffmanCodeBits->first ;
   
   if (current_node == NULL) {
      fprintf(stderr, "Empty huffman code! Probably invalid input file.\n") ;
      exit (EXIT_FAILURE) ;
   }
   
   while (current_node != NULL) {
      putbit (outp_file, (current_node->bit)) ;
      current_node = current_node->next ;
   }
}



static void
print_freq_table (int *char_frequency)
{
  int i = 0;  

  printf ("Frequencies: \n");

  for ( i = 0 ; i < MAX_CHAR; i++)
  {
    if (char_frequency[i] > 0)
    {
        printf ("%3d:  %d\n", i , char_frequency[i]);
    }
  }
} 




/*-----------------------------------------------------------------------------
| Function: build_bst_priority_queue
|
| Description:
|  Builds a priority queue of huffman nodes, using a BST priority queue.
|
| Algorithm:
|  Create a new huffman leaf node for each ascii value.
|  Encapsulate that node in a BST node.
|  Insert that node into the BST priority queue.
|
| Parameters:
|  -        bstroot: Double pointer to the root of the BST.
|  - char_frequency: The frequency of the ascii values read in.
*----------------------------------------------------------------------------*/
static void 
build_bst_priority_queue 
(bstNode** bstroot, int* char_frequency)
{
   unsigned int i = 0 ;
   bstNode* newNode ;
   huffNode* newHuffNode ;
   
   for (i = 0; i < MAX_CHAR; i++) {
      if (char_frequency[i] > 0) {
         newHuffNode = create_huffnode (char_frequency[i], i, NULL, NULL) ;
         newNode = create_bst_node (&newHuffNode, NULL, NULL, NULL) ;         
         insert_bstnode (bstroot, newNode) ;
      }
   }
}



/*-----------------------------------------------------------------------------
| Function: create_huffmantree_with_bst
|
| Description:
|  Constructs a huffman tree from a BST priority queue of huffman nodes.
|
| Algorithm:
|  Remove the two lowest frequency huffman nodes from the priority queue.
|  Create a new huffman node that points to these two removed nodes.
|  Set the frequency of the new huffman node as the sum frequencies of the two
|  previously removed nodes.
|  Inserts the new huffman node into the priority queue.
|  This is repeated until there's only one huffman node in the priority queue.
|  This final node is removed from the BST and returned.
|
|  NOTE: The function for removing a BST node frees the memory used by that
|        node. See pqbst.c for more details.
|
| Parameters:
|  - bstroot: Double pointer to the root of the BST.
| 
| Returns: Pointer to the root of the huffman tree.
*----------------------------------------------------------------------------*/
static huffNode* 
create_huffmantree_with_bst
(bstNode** bstroot)
{
   unsigned int n = bst_size (*(bstroot)) ;
   unsigned int i = 1 ;
   huffNode* newHuffNode = NULL ;
   huffNode* x = NULL ;
   huffNode* y = NULL ;
   bstNode* newNode = NULL ;
   
   for (i = 1; i <= (n - 1); i++) {
      x = remove_priority_bstnode (bstroot) ;
      y = remove_priority_bstnode (bstroot) ;
      newHuffNode = create_huffnode ((x->freq + y->freq), NULL, x, y) ;
      newNode = create_bst_node (&newHuffNode, NULL, NULL, NULL) ;
      insert_bstnode (bstroot, newNode) ;
   }

   return (remove_priority_bstnode (bstroot)) ;
}


/* 
 * usage()
 *
 * print usage message 
 * (expected command-line options and associated arguments) 
 * and exit with error exit status by calling exit(EXIT_FAILURE)
 *
 * Parameters:
 *   None or Void.
 *
 * Return value:
 *   None or Void. 
 */
static void usage()
{
  fprintf(stderr, "Usage (Encoding): \n"
         "huffman -e [-f] [-c] [-o outputfile] inputfile\n"
         "Usage (Decoding) : \n"  
         "huffman -d [-f] [-c] [-o outputfile] inputfile\n");
  exit (EXIT_FAILURE);
}

