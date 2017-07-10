#/*=============================================================================
#+------------------------------------------------------------------------------
#|Author: Jithya Nanayakkara
#|Date: 4/12/2013
#|
#|Description:
#|   An online slang dictionary was saved as an html file from
#|   http://www.noslang.com/dictionary/full
#|   This program extracts the words and their meanings from this HTML file
#|   and dumps it into a binary pickle file as a dictionary in the form: 
#|   slangdict[word] = def
#|
#+============================================================================*

import re
import cPickle

slangdocfilename = "slang.htm"
slangdictfilename = "slangdict.pickle"
slangdict = {}

slangdocfile = open (slangdocfilename, 'r')
slangdictfile = open (slangdictfilename, 'wb')

slanghtml = ''.join(slangdocfile.readlines())
slang_names = re.findall('name="[^"]*', slanghtml)
slang_defs = re.findall('title="[^"]*', slanghtml)

i = 0
for sname in slang_names:
   sname = re.sub('name="', '', sname)
   sdef = re.sub('title="', '', slang_defs[i])
   sdef = sdef.split(' ')
   i += 1

   
   if ((len(sdef) == 1)):
      slangdict[sname] = sdef[0]
   else:
      slangdict[sname] = sname


cPickle.dump(slangdict, slangdictfile, cPickle.HIGHEST_PROTOCOL)
slangdocfile.close()
slangdictfile.close()


