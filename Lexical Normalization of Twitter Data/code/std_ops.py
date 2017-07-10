#*****************************************************************************
#* Functions for parsing corpuses and writing the processed output. 
#*****************************************************************************

import cPickle
import re
import sys   



##/*----------------------------------------------------------------------------
##| Function: LoadPickledCorpus
##|
##| Description: 
##|   Loads a pickled binary file containing a dictionary of tweets in the form 
##|   described in the Returns section.
##|
##| Parameters:
##|     filename - string. Name of the pickled file.
##|
##| Returns: A dictionary of tweets, in the form:
##|      tweets[id] = [('token1', 'NA'), ('token2', 'NA')...]
##|         where 'id' corresponds to a single tweet, in the order it was
##|         read from the corpus file.
##|         'NA' is a placeholder for the category of the word (IV, OOV or NO)
##*---------------------------------------------------------------------------*/
def LoadPickledCorpus (filename):
   pickle = open(filename, 'rb')
   dic = cPickle.load(pickle)
   pickle.close()
   
   return dic



##/*----------------------------------------------------------------------------
##| Function: WritePickle
##|
##| Description: 
##|   Writes an object into a binary pickle file.
##|
##| Parameters:
##|     filename - string. Name of the pickled file.
##|     obj - DS to be pickled.
##|
##| Returns: Nothing
##*---------------------------------------------------------------------------*/
def WritePickle (filename, obj):
   pickle = open(filename, 'wb')
   cPickle.dump(obj, cPickle.HIGHEST_PROTOCOL)
   pickle.close()



##/*----------------------------------------------------------------------------
##| Function: LoadCorpusTest
##|
##| Description: 
##|   Loads a given corpus test file. An example of its form is:
##|      3     (number of words in a tweet)
##|      a
##|      nice
##|      day
##|
##| Parameters:
##|     filename - string. Name of the corpus file, including extension
##|
##| Returns: A dictionary of tweets, in the form:
##|      tweets[id] = [('token1', 'NA'), ('token2', 'NA')...]
##|         where 'id' corresponds to a single tweet, in the order it was
##|         read from the corpus file.
##|         'NA' is a placeholder for the category of the word (IV, OOV or NO)
##*---------------------------------------------------------------------------*/
def LoadCorpusTest (filename):
   corpus = open (filename, 'r')
   index = -1
   no_of_tokens = 0
   tweets = {}

   for line in corpus:
      line = line.strip('\n')

      if (no_of_tokens == 0):
         index += 1
         tweets[index] = []
         try:
            no_of_tokens = int(line)
         except:
            print "Error: Expected an int.\n", sys.exc_info()[0]
            file.close()
      else:
         tweets[index].append((line,'NA'))
         no_of_tokens -= 1
         
   corpus.close()
   return tweets



##/*----------------------------------------------------------------------------
##| Function: LoadCorpusStats
##|
##| Description: 
##|   Loads a given corpus file whose tokens are categorized and returns
##|   a dictionary of the tokens in a form useful for collecting statistics. 
##|
##|   An example of the corpus form is:
##|      3              (number of words in a tweet)
##|      a  IV a        (the spaces are a single tab)  
##|      nice  IV nice
##|      day   IV day
##|
##| Parameters:
##|     filename - string. Name of the corpus file, including extension
##|
##| Returns: A dictionary of token categories, in the form:
##|      stats[CAT] = ['token1', 'token2',...]
##|         where 'CAT' corresponds to the token category (IV, OOV or NO).
##|         
##*---------------------------------------------------------------------------*/
def LoadCorpusStats (filename):
   file = open(filename, 'r')
   no_of_tokens = 0
   stats = {'IV':[], 'NO':[], 'OOV':[]}

   for line in file:
      m = re.findall(r"[^\t\n]+", line)

      if (no_of_tokens == 0):
         try:
            no_of_tokens = int(m[0])
         except:
            print "Error: Expected an int.\n", sys.exc_info()[0]
            file.close()
      else:
         stats[m[1]].append(m[0])
         no_of_tokens -= 1

   for key in stats.iterkeys():      
      stats[key].sort()

   file.close()
   return stats



##/*----------------------------------------------------------------------------
##| Function: WriteNormalizedCorpus
##|
##| Description: 
##|   Writes to file a dictionary of tweets. The format written to file is:
##|      3              (number of words in a tweet)
##|      a  IV a        (the spaces are a single tab)  
##|      nice  IV nice
##|      day   IV day
##|
##| Parameters:
##|      norm_tweets - A dictionary in the form tweets[id] = [(token, cat)..]
##|                    Contains the normalized tweets.
##|      filename - string. Name of the corpus file, including extension
##|      orig_tweets - A dictionary in the form tweets[id] = [(token, cat)..]
##|                    Contains the un-normalized (original) tweets used to make
##|                    the normalized tweets.
##|
##| Returns: Nothing         
##*---------------------------------------------------------------------------*/
def WriteNormalizedCorpus (norm_tweets, filename, orig_tweets):
   norm_corpus = open(filename, 'w')
   id = 0
   num_tweets = len(norm_tweets.keys())

   while (id <= (num_tweets - 1)):
      num_tokens = len(norm_tweets[id])
      norm_corpus.write(str(num_tokens)+'\n')
      i = 0

      for token_pair in norm_tweets[id]:
         norm_corpus.write(orig_tweets[id][i][0] + '\t' + token_pair[1] + '\t' + token_pair[0] + '\n')
         i += 1

      id += 1

   norm_corpus.close()   

