#include "invIndexHeader.h"
#include "OleanderStemmingLibrary/english_stem.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void HandleInvertedIndex(BplusTree T, int docCnt, FILE * fp);

void HandleInvertedIndex(BplusTree T, int docCnt, FILE * fp) {
    int i;
    int pre, cur;
    char tmp[MAXREADSTRLEN];


    pre = cur = 0;
    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {
        for (i = 0; i < strlen(tmp); i++) {
            if (ispunct(tmp[i]) || isspace(tmp[i])) {
                cur = i;
            }
            
        }        
    }
}