/* Functions for handing input and output of individual bits */

#define FLUSH_BYTE (-1)

/* Function getbit() reads the next unread bit from file fp.
 * Returns the value of the bit read, or EOF if end of file is reached.
 */
int getbit(FILE *fp);

/* Function putbit() writes a bit to the next position in the file fp
 * If bit is set to 1 or 0 then its value is written. If bit is set to
 * FLUSH_BYTE, the currently buffered byte is written to the file, and any
 * bits that haven't been set are taken as 0.
 */
void putbit(FILE *fp, int bit);

