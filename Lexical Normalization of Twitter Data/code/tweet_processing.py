#*****************************************************************************
#* Functions for processing tokens in tweets. It classifies them as
#* as IV, OOV or NO.
#* OOV tokens are candidates for normalization.
#*****************************************************************************
# StringMatcher to calculate Levenshtein Distance is from:
#  https://pypi.python.org/pypi/python-Levenshtein/
# The Windows binary used was obtained from:
#  http://www.lfd.uci.edu/~gohlke/pythonlibs/#python-levenshtein
#
# Soundex code is from http://rosettacode.org/wiki/Soundex#Python 
#
# PyEnchant can be found: http://pythonhosted.org/pyenchant/
# It uses the MySpell dictionary. To check this, execute the following:
#     import enchant
#     d = enchant.Dict("en_US")
#     d.provider

import enchant
import re
import std_ops
import StringMatcher
import Editex
from itertools import groupby



# From http://rosettacode.org/wiki/Soundex#Python 
def soundex(word):
   codes = ("bfpv","cgjkqsxz", "dt", "l", "mn", "r")
   soundDict = dict((ch, str(ix+1)) for ix,cod in enumerate(codes) for ch in cod)
   cmap2 = lambda kar: soundDict.get(kar, '9')
   sdx =  ''.join(cmap2(kar) for kar in word.lower())
   sdx2 = word[0].upper() + ''.join(k for k,g in list(groupby(sdx))[1:] if k!='9')
   sdx3 = sdx2[0:4].ljust(4,'0')

   return sdx3



##/*----------------------------------------------------------------------------
##| Function: RemoveMultiTokenWords
##|
##| Description: 
##| Checks a list of words which are multi-token (defined as having a space).
##| Returns a new list of words which are only single token.
##|
##| Parameters:
##|  wordlist - a list of words
##|
##| Returns: A list of single token words.
##*---------------------------------------------------------------------------*/
def RemoveMultiTokenWords (wordlist):
   temp = []
   for word in wordlist:
      if (word.find(' ') == -1):
         temp.append(word)

   return temp


##/*----------------------------------------------------------------------------
##| Function: LevDistCorrect
##|
##| Description: 
##| Calculates the soundex codes for a list of suggested words and a token.
##| The first soundex code of the suggested words to match the token
##| is returned. Otherwise the original token is returned.
##|
##| Parameters:
##|   suggestions - A list of strings.
##|   token - string to check.
##|
##| Returns: The first suggested word that matches the token's soundex code.
##|          If none exist, the original token is returned.
##*---------------------------------------------------------------------------*/
def SoundexCorrect (token, suggestions):
   token_sdex = soundex(token)

   for word in suggestions:
      if (token_sdex == soundex(word)):
         return word

   return token



##/*----------------------------------------------------------------------------
##| Function: LevDistCorrect
##|
##| Description: 
##| Calculates the edit distances between a list of suggested words and a token
##| using the Levenshtein algorithm. The suggested word with the smallest edit distance
##| is returned.
##|
##| Parameters:
##|   suggestions - A list of strings.
##|   token - string to check.
##|
##| Returns: The suggested word with the smallest Levenshtein edit distance 
##*---------------------------------------------------------------------------*/
def LevDistCorrect (token, suggestions):
   temp = StringMatcher.distance(token, suggestions[0])
   corrected_token = suggestions[0]

   for word in suggestions[1:]:
      dist = StringMatcher.distance(token, word)

      if (dist < temp):
         corrected_token = word

   return corrected_token



##/*----------------------------------------------------------------------------
##| Function: EditexDistCorrect
##|
##| Description: 
##| Calculates the edit distances between a list of suggested words and a token
##| using the editex algorithm. The suggested word with the smallest edit distance
##| is returned.
##|
##| Parameters:
##|   suggestions - A list of strings.
##|   token - string to check.
##|
##| Returns: The suggested word with the smallest editex distance 
##*---------------------------------------------------------------------------*/
def EditexDistCorrect (token, suggestions):
   temp = Editex.edit3ex(token, suggestions[0])
   corrected_token = suggestions[0]

   for word in suggestions[1:]:
      dist = Editex.edit3ex(token, word)

      if (dist < temp):
         corrected_token = word

   return corrected_token



##/*----------------------------------------------------------------------------
##| Function: AttemptCorrection
##|
##| Description: 
##|   Attempt to correct the spelling of an OOV token.
##|
##| Parameters:
##|   d - initialized pyenchant dictionary
##|   REP_pattern - the pattern of character repetitions to detect (in regex)
##|   token - string to check
##|   mode - What method should be chosen to select the closest match from
##|          pyenchant's list of suggestions:
##|            - 'default': the first suggestion by pyenchant
##|            - 'soundex': the first suggestion by pyenchant whose soundex code
##|                         matches the token's soundex code
##|            - 'levdist': the smallest edit distance between pyenchant's suggestions
##|                         and token is taken, using the levenshtein algorithm.
##|            - 'editex': the smallest edit distance between pyenchant's suggestions
##|                        and token is taken, using the editex algorithm.
##|
##| Returns: A string. The token (normalized if correction is successful, otherwise
##|                    unchanged)
##*---------------------------------------------------------------------------*/
def AttemptCorrection (token, d, REP_pattern, mode):
   token = CollapseRepetitions(token, REP_pattern)
   temp = RemoveMultiTokenWords ( d.suggest(token) )

   if (temp != []):
      if ((len(temp) == 1) or (mode == "default")):
         return temp[0]

      else:
         if (mode == "levdist"):
            return LevDistCorrect (token, temp)

         elif (mode == "soundex"):
            return SoundexCorrect (token, temp)

         elif (mode == "editex"):
            return EditexDistCorrect (token, temp)
         else:
            print "\nError: Wrong mode specification\n"
            sys.exit(1)
   else:
      return token
   


##/*----------------------------------------------------------------------------
##| Function: CheckSlang
##|
##| Description: 
##|   Checks a given token against a given slang dictionary.
##|
##| Parameters:
##|   slangdic - dictionary of slang -> meaning (single word)
##|   token - string to check
##|
##| Returns: Is Not a slang term: False
##|          Is a slang term: a string consisting of 1 token.
##*---------------------------------------------------------------------------*/
def CheckSlang (slangdic, token):
   if (slangdic.has_key(token)):
      return slangdic[token]
   else:
      return False
   


##/*----------------------------------------------------------------------------
##| Function: CollapseRepetitions
##|
##| Description: 
##|   Every character in a token that is repeated more than thrice consecutively
##|   is reduced to only two repetitions. Eg: 'gooooddd' -> 'goodd'
##|
##| Parameters:
##|   token - string to check and correct
##|   REP_pattern - compiled regex pattern to check for repeating char groups
##|
##| Returns: String (the token - modified if repetitions were found).
##*---------------------------------------------------------------------------*/
def CollapseRepetitions (token, REP_pattern):
   matches = re.findall(REP_pattern, token)

   for match in matches:
      token = re.sub(match + "{3,}", match+match, token)

   return token



#Debug function
def progress (id, length):
   print "Processed " + str(id) + " of " + str(length)



##/*----------------------------------------------------------------------------
##| Function: CheckCapitalized
##|
##| Description: 
##|   Check if a token exists in the pyenchant dictionary if it is 
##|   capitalized (because words such as 'tuesday' wouldn't exist
##|   in the dictionary unless properly capitalized ('Tuesday' or 
##|   'TUESDAY').
##|
##| Parameters:
##|   token - string to check
##|   d - pyenchant dictionary
##|
##| Returns: True or False (depending if token is in dictionary or not, respectively).
##*---------------------------------------------------------------------------*/
def CheckCapitalized(token, d):
   token = token.upper()
   return d.check(token)



##/*----------------------------------------------------------------------------
##| Function: CheckIV
##|
##| Description: 
##|  Checks if a given token is in the given dictionary.
##|
##| Parameters:
##|   token - string to check
##|   d - pyenchant dictionary
##|
##| Returns: True if in the dictionary
##|          False if not in the dictionary
##|          String 'NO' if the token consists of characters that cause
##|           the dictionary to fail.
##*---------------------------------------------------------------------------*/
def CheckIV (token, d):
   try:
      if (d.check(token) == True):
         return True
      else:
         return CheckCapitalized(token, d)
   except:
      return 'NO'



##/*----------------------------------------------------------------------------
##| Function: CheckIV
##|
##| Description: 
##|  Classifies the tokens of a given dictionary of tweets as NO, IV or OOV.
##|  Attempts to normalize OOV tokens.
##|
##| Parameters:
##|   tweets - A dictionary of tweets in the form:
##|               tweets[id] = [('token1', 'NA'), ('token2', 'NA')...]
##|   NO_pattern - precompiled regex of patterns to classify as NO
##|
##| Returns: Classified and Normalized tweets. eg:
##|            tweets[id] = [('token1', 'IV'), ('token2', 'OOV')...]
##*---------------------------------------------------------------------------*/
def ProcessTweets (tweets, NO_pattern, mode):  
   REP_pattern = re.compile(r"(\w)\1")
   d = enchant.Dict("en_US")      
   slangdic = std_ops.LoadPickledCorpus("slangdict.pickle")  
   length = len(tweets.keys())

   for id in tweets.iterkeys():
      progress (id, length)
      i = 0
      for token_pair in tweets[id]:
         clean_token = token_pair[0].replace('\\','\\')  #To handle issues with \

         #Check if token is NO based on precompiled regex pattern
         if (NO_pattern.match(clean_token) != None): 
            tweets[id][i] = (clean_token, 'NO')
         else:
            #Check if token is IV or NO
            flag = CheckIV(clean_token, d)

            if ((flag == True) and ((len(clean_token) > 1)) or (clean_token == 'I') or (clean_token == 'A') or (clean_token == 'a') or (clean_token == 'i')):              #Check if token is a legit word (IV)
               tweets[id][i] = (clean_token, 'IV')
            elif (flag == 'NO'):
               tweets[id][i] = (clean_token, 'NO')
            else:
               #Token is OOV. Attempt to normalize.
               temp = CheckSlang (slangdic, clean_token)

               if (temp != False):
                  clean_token = temp
               else:
                  clean_token = AttemptCorrection (clean_token, d, REP_pattern, mode)
               tweets[id][i] = (clean_token.lower(), 'OOV')
         i += 1

   return tweets