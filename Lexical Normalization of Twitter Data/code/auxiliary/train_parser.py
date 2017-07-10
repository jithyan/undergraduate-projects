#/*=============================================================================
#+------------------------------------------------------------------------------
#|Author: Jithya Nanayakkara
#|Date: 4/12/2013
#|
#|Description:
#|   Extracts the original twitter message from the given corpus.train file,
#|   so that the system can be tested and compared.
#|   It writes the twitter messages to a pickle file (binary format) in the form:
#|    messages[id] = [list of words]
#|   where 'id' corresponds to a single tweet, in the order found in the train file.
#|
#+============================================================================*
import re
import cPickle

filename = "corpus.train"
messages_file = open("train.pickle", "wb")
file = open(filename, 'r')
index = -1
no_of_tokens = 0
messages = {}

for line in file:
   line = line.strip('\n')
   m = re.findall("^[^\t]+", line)
   
   if (no_of_tokens == 0):
      index += 1
      messages[index] = []
      try:
         no_of_tokens = int(m[0])
      except:
         print "Error: Expected an int.\n", sys.exc_info()[0]
         file.close()
   else:
      messages[index].append((m[0],'NA'))
      no_of_tokens -= 1

cPickle.dump(messages, messages_file, cPickle.HIGHEST_PROTOCOL)
messages_file.close()
file.close()
