#*****************************************************************************
#* Executes the processing of a corpus of tweets and writes the result.
#*
#* Usage:
#*  -> INPUT_FN - the filename of the corpus you wish to process.
#*  -> OUTPUT_FN - the filename of the results to be written.
#*  -> NORMALIZE_MODE - the algorithm used for normalizing tokens. 
#*                      Has the following options:
#*            - 'default': Use the first suggestion by the pyenchant dictionary (MySpell)
#*            - 'soundex': the first suggestion by pyenchant whose soundex code
#*                         matches the token's soundex code
#*            - 'levdist': the smallest edit distance between pyenchant's suggestions
#*                         and token is taken, using the levenshtein algorithm.
# *           - 'editex': the smallest edit distance between pyenchant's suggestions
#*                        and token is taken, using the editex algorithm.
#*
#*
#* Notes:
#* PyEnchant can be found: http://pythonhosted.org/pyenchant/
#* It uses the MySpell dictionary. To check this, execute the following:
#*     import enchant
#*     d = enchant.Dict("en_US")
#*     d.provider
#*
#* ---------------------------------------------------------------------------
#* Author: Jithya Nanayakkara (342973)
#*****************************************************************************
import std_ops
import corpstats
import enchant
import re
import tweet_processing as tp


def main():
   #FN = filename
   NORMALIZE_MODE = 'default'    #can be 'default', 'soundex', 'editex' levdist'
   INPUT_FN = 'corpus.test'
   INPUT_PICKLE = 'train.pickle' #To avoid repeated parsing of the train file during testing
   OUTPUT_FN = 'output_' + NORMALIZE_MODE + '.txt'
   
   
   # This block of code is the Project 1 output
      # What tokens should be classified as NO:
   NO_pattern = re.compile(r"(.\.+$)|(\d+$)|(^@.+)|(http:.*)|(ftp:.*)|(www\..*)|(#.*)|([\d`\"=_!@$%^&*()-+.{}:;'\[\]\\<>?,./]+$)")

   #tweets = std_ops.LoadPickledCorpus(INPUT_PICKLE)
   tweets = std_ops.LoadCorpusTest(INPUT_FN)

   tweets = tp.ProcessTweets (tweets, NO_pattern, NORMALIZE_MODE)

   #std_ops.WriteNormalizedCorpus (tweets, OUTPUT_FN, std_ops.LoadPickledCorpus(INPUT_PICKLE))
   std_ops.WriteNormalizedCorpus (tweets, OUTPUT_FN, std_ops.LoadCorpusTest(INPUT_FN))

   ##-----------------------------------------------------------------------------------------------
   ## The following blocks of code are to help analyze the output - they can be safely removed.

   #   #This block prints a list of tokens in their categories as an HTML table
   #html = corpstats.TokenList (std_ops.LoadCorpusStats(OUTPUT_FN))
   #corpstats.WriteHTML (html, 'list_'+NORMALIZE_MODE)   
   #html = corpstats.TokenList(std_ops.LoadCorpusStats(INPUT_FN))
   #corpstats.WriteHTML (html, 'list-gold')

   ##This block of code lists words that have been normalized differently
   #corpstats.CompareNormalizations(INPUT_FN, OUTPUT_FN)
   #g = std_ops.LoadCorpusStats(INPUT_FN)
   #n = std_ops.LoadCorpusStats(OUTPUT_FN)

   ##Lists what tokens have been categorized differently.
   #corpstats.CompareCategories(n,g)


if __name__ == "__main__":
    main()

