import json

##/*----------------------------------------------------------------------------
##| Function: expand
##|
##| Description:
##|     Expands a list of query terms based on its own thesaurus.
##|     If the given list of query terms contains terms in the thesaurus,
##|     the words associated with that term in the thesaurus are added to the list.
##|
##| Parameters:
##|     qlist: List of query terms
##|
##| Returns: An expanded list of query terms (the expansion only occurs if the
##|            terms in qlist exist in the thesaurus).
##*---------------------------------------------------------------------------*/  
def expand (qlist):
   thesaurus = json.loads(open("thesaurus.json").read())
   updated = False
   expanded_qlist = qlist

   for qterm in qlist:
      if ((thesaurus.has_key(qterm))):
         expanded_qlist = expanded_qlist + (thesaurus[qterm])
         updated = True
      
   if (updated == True):
      expanded_qlist = set(expanded_qlist)
      expanded_qlist = list(expanded_qlist)
   
   return expanded_qlist
      
      
   
