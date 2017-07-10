List of files and folders
--------------------------

output-files (folder):
The output files for the queries in TREC format.

thesaurus-builder (folder):
Contains the script for building the thesaurus used for 
query expansion. It is only included if you are interested 
in looking at it.
The thesaurus.JSON file is already included so you can use 
the query expansion add-on without having to run the
thesaurus building script.

vbjdn_report_ktproj2.pdf:
The report for project 2.

thesaurus.json:
The thesaurus used for the query expansion add-on.

term_processing.py:
The term processing add-on.

stopwords.txt:
The list of stop words to remove. Used by the term processing add-on.

spydr.py
Modified Tim's original code. Includes the third add-on (pseudo relevance
feedback) in the code.

query_expand.py:
The query expansion using a thesaurus add-on.

porter.py:
Contains code for the Porter stemmer. Not my code, got it off the net from:
http://pypi.python.org/pypi/stemming/1.0
Used by term_processing.py

batch-trec-spydr:
The modified bash script to run Spydr.


How to use
-----------

Add the following switches to the bash script in order to enable the add-ons:
-q (Query expansion using a thesaurus)
-p (Pseudo relevance feedback)
-e (Token processing)


Example:
nice sh batch-trec-spydr -q -p -e -r 'test-all-addons'

