
/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX(A, B)         ((A) > (B) ? (A) : (B))
#define MIN(A, B)         ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)  ((A) <= (X) && (X) <= (B))
#define LENGTH(X)         (sizeof X / sizeof X[0])

void *ecalloc(size_t nmemb, size_t size);
char *strim(char *s);
int startwith(char *s, char *sub);
