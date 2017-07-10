================================================================================
Project 1: Lexical Normalization of Twitter Data
-------------------------------------------------
Name: Jithya Nanayakkara
ID: 342973
================================================================================
CONTENTS
--------
1. File List
2. Libraries, development details
3. Using the system
================================================================================

--------------------------------------------------------------------------------
1. File List
--------------------------------------------------------------------------------

A) The folders 'train-results' and 'test-results' contain the verbose results
   of running eval.pl over the processed train/test corpuses.

B) In the folder 'code':
   - main.py : The main script which executes the entire tweet processing 
               system.
               Running it will require editing of the file itself.
               
               
   - Editex.py : A python implementation of Justin Zobel's editex algorithm 
                (lifted from his C implementation available on his website).
                
              
   - corpstats.py : Should be ignored. Used for outputting stats of the results
                    of the system.
                    
   
   - std_ops.py : Contains functions for parsing the input file and writing the
                  output.
                  
                  
   - tweet_processing.py : All functions related to classifying and normalizing
                           the twitter tokens are here.
                           
                           
   - slangdict.pickle : Contains slang terms and their meaning from noslang.com.
                        Needed to execute the system. 
                        Slang terms whose meanings are multi-token are mapped
                        back to themselves (i.e. 'imma'->'imma').
                        See 'auxiliary' folder for script.   
                                            
   
   - train.pickle : If you wish to test the system on the corpus.train file you 
                    could either use this file (and the function 
                    std_ops.LoadPickledCorpus('train.pickle')) or create a train
                    file which does not have the annotations (the parsing 
                    function I wrote in std_ops cannot parse annotated 
                    corpuses).
                    
                    
   - auxiliary <folder> : Contains some scripts that were used to prepare the 
                          pickles used in the system -
                          > slang.htm: noslang.com webpage, complete listing
                                       of its slang dictionary.
                                       
                          > slang_builder.py: parses the slang.htm and dumps 
                                              the result in a pickle.
                                              
                          > train_parser.py: parses an annotated corpus and 
                                             dumps it into a pickle.

--------------------------------------------------------------------------------
2. Libraries, development details
--------------------------------------------------------------------------------                                             
Developed on Windows 8, Python 2.7 (32-bit).

External libraries used:
   - pyenchant: Uses the MySpell dictionary. Allows you to easily check if
                a word exists in the dictionary and it can provide an ordered
                list of speling suggestions.
                http://pythonhosted.org/pyenchant/
   
   - python-Levenshtein: Used to calculate the Levenshtein distance between
                         strings.
                         https://pypi.python.org/pypi/python-Levenshtein/



--------------------------------------------------------------------------------
3. Using the system
--------------------------------------------------------------------------------

To process an un-annotated corpus (like 'corpus.test') is straightforward:
   > Open main.py
   > In the function main():
      - Change 'NORMALIZE_MODE' to either 'default', 'soundex', 'levdist' or
        'editex'. This sets which algorithm the system uses to normalize tokens.
        Descriptions of them are in 'main.py'
      
      - Change 'INPUT_FN' to the name of the corpus you wish to test.
      
   > Run main.py. 
     For it to work, it needs 'slangdict.pickle' and the other pys to be in the 
     same directory. 'corpstats.py' and the folder 'auxiliary' are not required.
     

To process an annotated corpus (like 'corpus.train'), it's best to create an un-
annotated corpus file yourself.