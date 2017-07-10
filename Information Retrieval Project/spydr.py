#!/usr/bin/python


import re
import os
from collections import defaultdict
from heapq import heappush, heappop
from math import log, sqrt
from optparse import OptionParser
import query_expand
import term_processing






class Params:
    """
    Class for top-level system parameters
    """

    def __init__(self):
        # location of the document collection (assumption of one document per file, in TREC format)
        self.HOME = os.environ.get('HOME') + "/comp30018-proj2/"
        self.HOME = "./"

        # location of the document collection (assumption of one document per file, in TREC format)
        self.DOCSET = self.HOME + "docset/"

        # location of the query set (in a single file, in TREC format)
        self.TOPICS = self.HOME + "topics/Topics.trec"

        # maximum number of outputs per query
        self.MAXDOCS = 10

        # run name
        self.RUNID = "run1"

        self.QUERY_EXPAND = False
        self.TOKEN_PROCESS = False
        self.PSEUDO_RELEVANCE = False

        # command-line option definition
        self.parser = OptionParser()
        self.parser.add_option("-f", help="query topic file",
                               action="store", type="string", dest="topics")
        self.parser.add_option("-n", help="number of files to evaluate over",
                               action="store", type="int", dest="maxdocs")
        self.parser.add_option("-r", help="name of the current run",
                               action="store", type="string", dest="runid")
        self.parser.add_option("-d", help="(directory) location of the document collection",
                               action="store", type="string", dest="docset")
        self.parser.add_option("-p", help="enable pseudo-relevance feedback",
                               action="store_true", dest="pseudorelevance", default=False)

        self.parser.add_option("-e", help="enable extra token processing",
                               action="store_true", dest="tokenprocess", default=False)

        self.parser.add_option("-q", help="enable query expansion",
                               action="store_true", dest="queryexpand", default=False)
        # change system parameters based on any command-line options
        (options, args) = self.parser.parse_args()
        if options.topics:
            self.TOPICS = options.topics
        if options.maxdocs:
            self.MAXDOCS = options.maxdocs
        if options.runid:
            self.RUNID = options.runid
        if options.docset:
            self.DOCSET = options.docset
        if options.queryexpand:
           self.QUERY_EXPAND = options.queryexpand
        if options.pseudorelevance:
           self.PSEUDO_RELEVANCE = options.pseudorelevance
        if options.tokenprocess:
           self.TOKEN_PROCESS = options.tokenprocess






class Docset:
    """
    Class for parsing and indexing a document collection
    """

    def __init__(self,params):
        """
        Initialise various document collection data structures and regexes
        """
        self.docno_search = re.compile('^<DOCNO>(.+?)</DOCNO>\n$')
        self.docid_search = re.compile('^<DOCID>\s*(.+?)\s*</DOCID>\n$')
        self.tf = defaultdict(dict) # dictionary for TF statistics
        self.df = defaultdict(int) # dictionary for DF statistics
        self.doccount = 0.0 # number of documents in collection
        self.params = params # system params

        
    def get_terms(self,fp):
        """
        Return a single string containing the entire document, stripped of
        line breaks, markup and punctuation
        """
        text = ""
        # read in from 'fp' up until </DOC> (on a single line)

        while True:
            currline = fp.readline()
            if not currline or currline == "</DOC>\n": break
            text += currline

        # remove all newlines and tabs
        text = re.sub('\n|\t', ' ', text)

        # remove all markup
        text = re.sub(r'<.*?>', ' ', text)

        # remove all punctuation
        text = re.sub('[\.\,\:\;\"\'\!\?\(\)\[\]\{\}]', '', text)

        return text


    def create_index(self):
        """
        Index all documents in the collection via 'tf', 'df' and 'doccount'
        """
        # read in each file from 'self.params.DOCSET'
        for filename in os.listdir(self.params.DOCSET):
            # ignore any subversion directories
            if filename <> ".svn":
                fp = open(self.params.DOCSET+filename)
                while fp:
                    # look for next document, and return if none
                    while True:
                        currline = fp.readline()
                        if currline == '<DOC>\n': break
                        if not currline: return

                    # parse <DOCNO> line
                    m = self.docno_search.match(fp.readline())
                    assert m, filename
                    docid = m.group(1)

                    self.doccount += 1

                    # update tf and df statistics
                    terms = self.get_terms(fp).split()

                    if (self.params.TOKEN_PROCESS):
                       terms = term_processing.process_words(terms)

                    for term in terms:
                        if term not in self.tf[docid]:
                            self.tf[docid][term] = 0
                            self.df[term] += 1
                        self.tf[docid][term] += 1
                
                    # normalise the document vector to unit length
                    veclen = 0
                    for term in self.tf[docid]:
                        veclen += self.tf[docid][term]**2
                    veclen = sqrt(veclen)
                    for term in self.tf[docid]:
                        self.tf[docid][term] /= veclen


    def docids(self):
        """
        Return a list of docids for all documents in the collection
        """
        return self.tf.keys()


    def tfidf(self,docid,term):
        """
        Calcualte the tf-idf score for 'term' in document 'docid'
        """
        if term in self.tf[docid]:
            tf = self.tf[docid][term]
        else:
            return 0

        # basic definition of tf-idf from 352 lecture notes
        return tf * log(self.doccount/self.df[term],2)




class Query:
    """
    Class for querying an indexed document collection
    """

    def __init__(self,params):
        self.params = params
        self.qfp = open(self.params.TOPICS)
        self.qno_search = re.compile('^<num>(\S+)</num>\n$')
        self.qtitle_search = re.compile('^<title>(.+?)</title>\n$')

    def get_queries(self):
        queries = []

        # parse topics file, appending each query to 'queries'
        while (True):
            currline = self.qfp.readline()
            if not currline: break
            assert currline == '<top>\n'
            m = self.qno_search.match(self.qfp.readline())
            assert m
            qid = m.group(1)
            m = self.qtitle_search.match(self.qfp.readline())
            assert m
            qlist = []
            for qterm in re.split('\W+', m.group(1)):
                qlist.append(qterm)
            assert self.qfp.readline() == '</top>\n'

            #Perform processing from addons
            if (self.params.QUERY_EXPAND):
               qlist = query_expand.expand (qlist)
            if (self.params.TOKEN_PROCESS):
               qlist = term_processing.process_words (qlist)

            queries.append((qid,qlist))
            assert self.qfp.readline() == '\n'
        return queries


##/*----------------------------------------------------------------------------
##| This contains the original spydr code for processing document similarity,
##| it has been shifted to a function so it could be reused easily.
##*---------------------------------------------------------------------------*/  
def process_similarity (d, qlist):
        docheap = []
        for docid in d.docids():
            # calculate similarity for current document, via weighted cosine similarity
            sim = 0
            for qterm in qlist:
                term_weight = d.tfidf(docid,qterm)
                sim += term_weight
            sim /= sqrt(len(qlist))

            # store the similarity in 'docheap' [n.b. store complement of similarity, so most similar
            # documents are at the top of the heap, due to heap being *min*-heap]
            heappush(docheap,(1-sim,docid))   
        
        return docheap
    


##/*----------------------------------------------------------------------------
##| Function: expand
##|
##| Description:
##|     Performs a modified formed of pseudo relevance feedback for a given
##|     ranked collection of documents.
##|
##| Parameters:
##|     qterms: A list of query terms
##|     docheap: A minimum value document heap, containing the top returned 
##|              documents, with the top ranked document being the first in
##|              the heap.
##|     MAXDOCS: The number of documents returned
##|     d: The parsed and indexed docset
##|
##| Returns: An expanded list of query terms.
##*---------------------------------------------------------------------------*/     
def pseudo_relevance (docheap, qterms, MAXDOCS, d):
        #There must be at least 8 documents for pseudo relevance to be performed      
        if (MAXDOCS <= 7):
           return qterms

        top_docs = set()
        bad_qterms = set()
        qterms = set(qterms)
        limit = (MAXDOCS/4) + 1
        limit2 = limit - 1
        
        for i in xrange(0,limit):
         dist,docid = heappop (docheap)
         if (i < limit2):
            top_docs = top_docs.union(set(d.tf[docid].keys()))
         else:
            bad_qterms = bad_qterms.union(set(d.tf[docid].keys()))
                
        good_qterms = top_docs.intersection(qterms)
        qterms = top_docs.union(qterms)                
        qterms = qterms - bad_qterms       
        qterms = qterms.union(good_qterms)  
        
        return list(qterms)

   
    
def main():
    # initialise the system params, and parse any command-line options
    params = Params()

    myfile = open("o.txt","w")
    if (params.QUERY_EXPAND):
       myfile.write("query expanded\n")
    if (params.TOKEN_PROCESS):
       myfile.write("process tokens\n")
    if (params.PSEUDO_RELEVANCE):
       myfile.write("relevance feedback\n")
    myfile.close()

    # index the document collection
    d = Docset(params)
    d.create_index()


    # parse the topics file
    q = Query(params)

    # iterate through topics, scoring each document against the topic and storing the score in a (min-)heap
    for (qid,qlist) in q.get_queries():
        docheap = process_similarity (d, qlist) # used to store the document scores [n.b. *min*-heap]                      
        if (params.PSEUDO_RELEVANCE):
            docheap = process_similarity (d, pseudo_relevance (docheap, qlist, params.MAXDOCS, d))

        # retrieve the top-MAXDOCS documents and print out in TREC format
        for i in xrange(0,params.MAXDOCS):
            dist,docid = heappop(docheap)
            print "%s\tQ0\t%s\t%d\t%f\t%s" % (qid,docid,i+1,1-dist,params.RUNID)



if __name__ == "__main__":
    main()
