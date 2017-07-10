###############################################################################
# Makefile for Project 2 
# COMP20003 , 2011 
# Udaya
# Makefile for building the project 2 huffman program
# You can easily extend this Makefile to make other programs
# by adding/changing the list of .h and .c files to HDRS and SRCS, and
# adding libraries to LDLIBS and LDLIBPATH if necessary.
#
# The default target "all" will compile and link the program.
#
#
# The "tags" target will build the TAGS file for EMACS.
# If you use vi or vim and you want to use ctags or whatever, you will
# have to change this appopriately.
#
# The "clean" target will remove object and executable files.
#
# The "submit" and "verify" targets will help you submit the project.
#
# The _XOPEN_SOURCE and _XOPEN_SOURCE_EXTENDED defines are set to 1 to
# to allow certain prototypes to be included properly and indicate
# that your program is to work with the X/Open XPG4v2 (SUS)
# standard. Specifically, this way getopt() will compile and work
# nicely with no warnings on Solaris, and also allow you to use
# strcasecmp() and strdup() with no warnings. I don't recommend that
# you mess with this unless you know what you're doing.  See "man
# standards" on a Solaris box if you dare.
#
#
###############################################################################

HDRS       = bit_io.h util.h pqsortedlist.h huffmantree.h codebitqueue.h pqbst.h
SRCS       = huffman.c bit_io.c util.c pqsortedlist.c huffmantree.c codebitqueue.c pqbst.c
OBJS       = $(SRCS:.c=.o) 


CC         = gcc
CPPFLAGS   = -D_XOPEN_SOURCE=1 -D_XOPEN_SOURCE_EXTENDED=1
CDEBUG     = -g -O0
CFLAGS     = $(CDEBUG) -ansi -pedantic -Wall
#              the following warnings are not implied by -Wall
CFLAGS     += -Wextra -Wfloat-equal  \
              -Wdeclaration-after-statement -Wundef -Wshadow \
              -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align\
              -Wwrite-strings -Wmissing-prototypes \
              -Wmissing-declarations -Wunreachable-code


LD         = gcc
LDFLAGS    = -g
LDLIBPATH  = 
LDLIBS     = 


# Program to build TAGS file for EMACS
MAKETAGS   = etags


all: huffman

huffman: $(OBJS)
	$(LD) -o huffman $(OBJS) $(LDFLAGS) $(LDLIBPATH) $(LDLIBS)


# make the TAGS file for EMACS
tags:
	$(MAKETAGS) $(SRCS) $(HDRS)

clean:
	$(RM) $(OBJS)
	$(RM) huffman

submit:
	submit comp20003 proj2 $(SRCS) $(HDRS) Makefile report.txt

verify:
	verify comp20003 proj2

