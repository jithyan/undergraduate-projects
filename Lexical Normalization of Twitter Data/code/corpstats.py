#*****************************************************************************
#* Functions for writing comparisons and statistics of the output of
#* the processed corpuses.
#* This file should be ignored as it does not affect the processing of tweets
#* in anyway.
#* 
#*****************************************************************************

import std_ops
import re



##/*----------------------------------------------------------------------------
##| Function: TokenList
##|
##| Description: 
##|   Generates an HTML table which displays the token count in each token
##|   category (including the number of unique tokens) and another table
##|   that lists the unique tokens for each token category.
##|
##| Parameters:
##|   stats - a dictionary of token categories in the form:
##|           stats[CAT] = [token1, token2...]
##|            where CAT is the token category ('IV', 'OOV', 'NO')
##|
##| Returns: A string containing html tables
##*---------------------------------------------------------------------------*/
def TokenList (stats):
   html = '<table border="1"><tr><th>Word Count</th></tr>'
   html += '<tr><th> </th><th>IV</th><th>NO</th><th>OOV</th></tr>'
   html += '<tr><th>Num Words</th><td>' + str(len(stats['IV'])) + '</td>'
   html += '<td>' + str(len(stats['NO'])) + '</td>' + '<td>' + str(len(stats['OOV'])) + '</td></tr>'

   unique_stats = {'IV':set(stats['IV']), 'NO':set(stats['NO']), 'OOV':set(stats['OOV'])}

   html += '<tr><th>Num Unique Words</th><td>' + str(len(unique_stats['IV'])) + '</td>'
   html += '<td>' + str(len(unique_stats['NO'])) + '</td>' + '<td>' + str(len(unique_stats['OOV'])) + '</td></tr>'
   html += '</table>'

   html += '<br/><table border="1"><tr><th>IV</th></tr>'
   for word in unique_stats['IV']:
      html += '<tr><td>' + word + '</td></tr>'

   html += '<tr><th>NO</th></tr>'
   for word in unique_stats['NO']:
      html += '<tr><td>' + word + '</td></tr>'

   html += '<tr><th>OOV</th></tr>'
   for word in unique_stats['OOV']:
      html += '<tr><td>' + word + '</td></tr>'

   html += '</table><br/>'

   return html



##/*----------------------------------------------------------------------------
##| Function: WriteHTML
##|
##| Description: 
##|   Writes an HTML file, enclosing the given string input in the <html> & 
##|   <body> tags.
##|
##| Parameters:
##|   html - a string which should contain the html you want to write.
##|   filename - name of the html file.
##|
##| Returns: Nothing
##*---------------------------------------------------------------------------*/
def WriteHTML (html, filename):
   html_file = open(filename+'.html', 'w')
   html_file.write('<html><body>')
   html_file.write(html)
   html_file.write('</body></html>')
   html_file.close()



##/*----------------------------------------------------------------------------
##| Function: CompareNormalizations
##|
##| Description: 
##|   Lists the tokens that have been normalized differently between two 
##|   two outputs. Output written to file.
##|
##| Parameters:
##|   filename1, filename2 - String. Filenames of the corpuses
##|
##| Returns: Nothing
##*---------------------------------------------------------------------------*/
def CompareNormalizations (filename1, filename2):
   def parse(filename):
      no_of_tokens = 0
      stats = []
      file = open(filename, "r")

      for line in file:
         m = re.findall(r"[^\t\n]+", line)

         if (no_of_tokens == 0):
            try:
               no_of_tokens = int(m[0])
            except:
               print "Error: Expected an int.\n", sys.exc_info()[0]
               file.close()
               break
         else:
            if (m[1] == 'OOV'):
               stats.append(m[2])
            no_of_tokens -= 1          
      stats.sort()
      file.close()

      return set(stats)

   output = open("comparison.txt", "w")

   s1 = parse(filename1)
   s2 = parse(filename2)
   f1_diff_f2 = (s1 - s2)
   f2_diff_f1 = (s2 - s1)
   output.write("OOV terms in " + filename1 + " not " + filename2 + ":\n\n")

   for item in f1_diff_f2:
      output.write(item + "\n")
   output.write("\nOOV terms in " + filename2 + " not " + filename1 + ":\n\n")

   for item in f2_diff_f1:
      output.write(item + "\n")
   output.close()



##/*----------------------------------------------------------------------------
##| Function: CompareCategories
##|
##| Description: 
##|   Lists the tokens that have been classified into different categories
##|   between two token lists. The tokens listed are the ones which haven't
##|   been normalized. Output written to file.
##|
##| Parameters:
##|   norm, gold - A dictionary of token categories, in the form:
##|                  stats[CAT] = ['token1', 'token2',...]
##|                     where 'CAT' corresponds to the token category (IV, OOV or NO).
##|
##| Returns: Nothing
##*---------------------------------------------------------------------------*/
def CompareCategories (norm, gold):
   def WriteCatDiffs (category, norm, gold):
      file = open("stats_"+category+".txt", "w")

      file.write ("Gold classed as " + category +" but not Norm:\n\n")
      diff = gold-norm
      for token in diff:
         file.write(token + '\n')

      file.write("\n\nNorm classed as " + category + " but not Gold:\n\n")
      diff = norm-gold
      for token in diff:
         file.write(token + '\n')
      file.close()

   for cat in norm.iterkeys():
      WriteCatDiffs (cat, set(norm[cat]), set(gold[cat]))

