import porter

##/*----------------------------------------------------------------------------
##| Function: load_stopwords
##|
##| Description: 
##|     Loads stopwords from a file. The stopwords were taken from:
##|     http://www.ranks.nl/resources/stopwords.html
##|
##| Parameters:
##|     None.
##|
##| Returns: A list of stopwords
##*---------------------------------------------------------------------------*/
def load_stopwords ():
    stopwords_file = open ('stopwords.txt', 'rU')
    stopwords = []

    for line in stopwords_file:
        stopwords.append (line.strip('\n'))

    return stopwords
    
    
##/*----------------------------------------------------------------------------
##| Function: process_words
##|
##| Description:
##|     Takes a list of words and then:
##|         > Splits hyphenated words into separate words
##|         > removes duplicate words 
##|         > makes all words lower case
##|         > stems each word using the porter algorithm
##|         > removes stopwords
##|
##| Parameters:
##|     word_list: A list of words
##|
##| Returns: A list of words
##*---------------------------------------------------------------------------*/    
def process_words (word_list):
    new_word_list = []      
    stop_words = load_stopwords()

    list_length = len(word_list)
    for i in xrange(0, list_length):
      new_word_list.extend(word_list[i].split('-'))
    
    word_list = list(set(new_word_list))
    list_length = len(word_list)
    
    for i in xrange(0, list_length):
      if (word_list[i] not in stop_words):
         new_word_list.append(word_list[i])
    
    word_list = new_word_list
    list_length = len(word_list)    
      
    for i in xrange(0, list_length):
        word_list[i] = word_list[i].lower()
        word_list[i] = porter.stem(word_list[i])
        
    return word_list
