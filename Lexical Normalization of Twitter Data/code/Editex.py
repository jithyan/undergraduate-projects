#*****************************************************************************
#* Justin Zobel wrote an editex program in C. The source code was obtained from:
#* http://ww2.cs.mu.oz.au/~jz/resources/vrank.zip
#*
#* The following python code is a conversion of his C source code. 
#* All comments enclosed within /* */ are Justin's original comments.
#*****************************************************************************





# /* Phonetic edit distance: cost of insertion, deletion, swap is based on the
#   array pairvals.  Adjacent identical letters can be deleted for free. */

STRLEN = 60
true = 1
false	= 0
SILENTGROUP = 7
SMALLP = 2
BIGP = 3
B = BIGP
T = SMALLP

groupsof = [	
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  0,  1,  2,  7,  0,  2,  2,  4,  5,  5,  0,
	 1,  2,  6,  2,  3,  0,  1,  0,  2,  0,  2, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  0,  1,  2,  0,  0,  2,  2,  4,  5,  5,  0,
	 1,  2,  6,  2,  3,  0,  1,  7,  2,  0,  2, -1, -1, -1, -1, -1 ]

#             a b c  d e f  g h i  j k l  m n o  p q r  s t u  v w x  y z X
pairvals =  [[0,B,B, B,T,B, B,B,T, B,B,B, B,B,T, B,B,B, B,B,T, B,B,B, T,B,B ], #/* a */
            [ B,0,B, B,B,B, B,B,B, B,B,B, B,B,B, T,B,B, B,B,B, B,B,B, B,B,B ], #/* b */
            [ B,B,0, B,B,B, B,B,B, B,T,B, B,B,B, B,T,B, T,B,B, B,B,B, B,T,B ], #/* c */

[ B,B,B, 0,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,T,B, B,B,B, B,B,B ],
[ T,B,B, B,0,B, B,B,T, B,B,B, B,B,T, B,B,B, B,B,T, B,B,B, T,B,B ],
[ B,B,B, B,B,0, B,B,B, B,B,B, B,B,B, T,B,B, B,B,B, T,B,B, B,B,B ],

[ B,B,B, B,B,B, 0,B,B, T,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B ],
[ B,B,B, B,B,B, B,0,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B ],
[ T,B,B, B,T,B, B,B,0, B,B,B, B,B,T, B,B,B, B,B,T, B,B,B, T,B,B ],

[ B,B,B, B,B,B, T,B,B, 0,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B ],
[ B,B,T, B,B,B, B,B,B, B,0,B, B,B,B, B,T,B, B,B,B, B,B,B, B,B,B ],
[ B,B,B, B,B,B, B,B,B, B,B,0, B,B,B, B,B,T, B,B,B, B,B,B, B,B,B ],

[ B,B,B, B,B,B, B,B,B, B,B,B, 0,T,B, B,B,B, B,B,B, B,B,B, B,B,B ],
[ B,B,B, B,B,B, B,B,B, B,B,B, T,0,B, B,B,B, B,B,B, B,B,B, B,B,B ],
[ T,B,B, B,T,B, B,B,T, B,B,B, B,B,0, B,B,B, B,B,T, B,B,B, T,B,B ],

[ B,T,B, B,B,T, B,B,B, B,B,B, B,B,B, 0,B,B, B,B,B, T,B,B, B,B,B ],
[ B,B,T, B,B,B, B,B,B, B,T,B, B,B,B, B,0,B, B,B,B, B,B,B, B,B,B ],
[ B,B,B, B,B,B, B,B,B, B,B,T, B,B,B, B,B,0, B,B,B, B,B,B, B,B,B ],

[ B,B,T, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, 0,B,B, B,B,T, B,T,B ],
[ B,B,B, T,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,0,B, B,B,B, B,B,B ],
[ T,B,B, B,T,B, B,B,T, B,B,B, B,B,T, B,B,B, B,B,0, B,B,B, T,B,B ],

[ B,B,B, B,B,T, B,B,B, B,B,B, B,B,B, T,B,B, B,B,B, 0,B,B, B,B,B ],
[ B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,0,B, B,B,B ],
[ B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, T,B,B, B,B,0, B,T,B ],

[ T,B,B, B,T,B, B,B,T, B,B,B, B,B,T, B,B,B, B,B,T, B,B,B, 0,B,B ],
[ B,B,T, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, T,B,B, B,B,T, B,0,B ],# /* z */
[ B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B, B,B,B ]]# /* X */

#/* Soundex mappings:
#    abcdefghijklmnopqrstuvwxyz
#    01230120022455012623010202 */

#/* My mappings:
#    abcdefghijklmnopqrstuvwxyz
#    01230127022455012623017202 */

#/* cost of a replacement */
def reprate(c1, c2):
    return (pairvals[c1][c2])


#/* cost of a deletion */
def delrate(c1, c2):
    if( c1 == c2 ):
       return(0)
    elif( groupsof[c2+ord('a')] == SILENTGROUP ):
       return(SMALLP)
    else:
       return(pairvals[c1][c2])


def min3(a, b, c):
    if( a > b ):
       if (c > b):
          return b
       else:
          return c
    else:
      if (c > a):
         return a
      else:
         return c


# Params:
#  in1, in2 - strings to compare
# Return an int. The distance between the two strings.

def edit3ex(in1, in2):
   #Initialize a 61x61 square matrix of 0s
    F = [[0 for i in range(STRLEN+1)] for j in range(STRLEN+1)]

    l1 = len(in1)
    l2 = len(in2)
    inc1 = 0
    inc2 = 0 
    diag = 0
    sum = 0
    s1 = []
    s2 = []

    #/* manipulate strings to simplify comparison */
    # Convert to string to lowercase, replace all non-alphabet chars with 'z'
    in1 = in1.lower()
    in2 = in2.lower()

    for c in in1:
       if (c.isalpha() == True):
         s1.append(ord(c)-ord('a'))
       else:
         s1.append(26)

    for c in in2:
       if (c.isalpha() == True):
         s2.append(ord(c)-ord('a'))
       else:
         s2.append(26)


    F[0][0] = 0
    F[1][0] = BIGP

    i = 2
    sum = BIGP
    while (i <= l1):
       sum += delrate(s1[i-2], s1[i-1])
       F[i][0] = sum;
       i += 1

    F[0][1] = BIGP;

    j = 2
    sum = BIGP
    while (j <= l2):
       sum += delrate(s2[j-2], s2[j-1])
       F[0][j] = sum;
       j += 1

    i = 1
    while (i <= l1):
       j = 1
       while (j <= l2):
          if (i == 1):
             inc1 = BIGP
          else:
             inc1 = delrate(s1[i-2], s1[i-1])

          if (j == 1):
             inc2 = BIGP
          else:
             delrate(s2[j-2], s2[j-1])

          diag = reprate(s1[i-1], s2[j-1])
          F[i][j] = min3(F[i-1][j] + inc1, F[i][j-1] + inc2, F[i-1][j-1] + diag)

          j += 1
       i += 1

    
    return F[l1][l2]
