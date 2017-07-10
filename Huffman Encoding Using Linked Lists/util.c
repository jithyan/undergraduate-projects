/**
 * Utility functions.
 *
 * Author: Steven Bird (2006),
Modified Sep 2011 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "util.h"

void *
safe_malloc(size_t size)
{
  void *mem = NULL;

  if ((mem = malloc(size)) == NULL)
  {
    (void)fprintf(stderr, "Cannot allocate memory.\n");
    exit(EXIT_FAILURE);
  }
    
  return mem;
}

FILE *
safe_fopen(const char *path, const char *mode)
{
  FILE *fp = NULL;

  assert(path != NULL);
  assert(mode != NULL);

  if ((fp = fopen(path, mode)) == NULL)
  {
    (void)fprintf(stderr, "Error while attempting to try open");
    (void)fprintf(stderr, " file %s: %s\n", path, strerror(errno));
    exit(EXIT_FAILURE);
  }
    
  return fp;
}   
