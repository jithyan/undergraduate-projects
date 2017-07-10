/* Functions for handing input and output of individual bits */

#include <stdio.h>
#include "bit_io.h"

int
getbit(FILE *fp) {
    static unsigned int c;
    static int remaining = 0;
    int bit;

    if (remaining == 0) {
        c = fgetc(fp);
        
        if ((int)c == EOF) 
            return EOF;

        remaining = 8;
    }

    bit = c & 128;
    c <<= 1;
    remaining--;
    
    return (bit != 0);
}

void
putbit(FILE *fp, int bit) {
    static unsigned int c = 0;
    static int remaining = 8;
    if(bit != FLUSH_BYTE) {
        c <<= 1;
        c |= (unsigned int)bit;
        
        remaining--;    
    }
    
    if(remaining == 0 || (bit == FLUSH_BYTE && remaining != 8)) {
        c <<= remaining;
          
        fputc(c, fp);
        remaining = 8;
        c = 0;
    }
}


