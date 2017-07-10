
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

/**
 * malloc, exiting on error.
 */
extern void *safe_malloc(size_t size);

/**
 * fopen, exiting on error.
 */
extern FILE *safe_fopen(const char *path, const char *mode);

#endif
