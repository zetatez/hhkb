
/* See LICENSE file for copyright and license details. */

#include "utils.h"

void *ecalloc(size_t nmemb, size_t size) {
    void *p;
    if (!(p = calloc(nmemb, size))) {
        puts("die calloc");
        exit(1);
    }
    return p;
}

char *strim(char *s) {
    char *sp, *ep;
    sp = s;
    ep = s + strlen(s) - 1;
    while(*sp && isspace(*sp)) sp++;
    while(ep > sp && isspace(*ep)) *ep--='\0';
    return sp;
}

int startwith(char *s, char *sub) {
    if (strlen(s) < strlen(sub))
        return 0;

    for (int i = 0; i < strlen(sub); i++) {
        if (s[i] != sub[i])
            return 0;
    }

    return 1;
}
