
#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>


// void GenerateInvertedIndex(BplusTree T, int docCnt, FILE * fp);

void GenerateInvertedIndex(BplusTree T, int docCnt, FILE * fp) {
    int i;
    int pre, cur;
    char tmp[MAXREADSTRLEN];
    char term[MAXREADSTRLEN];
    bool isDuplicated;
    NodeBP nodebp;
    stemming::english_stem<> StemEnglish;

    pre = cur = 0;
    while (fgets(tmp, MAXREADSTRLEN - 1, fp) != NULL) {
        for (i = 0; i < strlen(tmp); i++) {
            if (ispunct(tmp[i]) || isspace(tmp[i])) {
                cur = i;
                strncpy(term, tmp + pre, cur - pre);
                term[cur - pre] = '\0';
                StemEnglish(term);
                isDuplicated = false;
                nodebp = FindBP(term, T, &isDuplicated);
                if (!isDuplicated) {
                    InsertBP(term, nodebp, T);
                }
                pre = cur;
            }
        }        
    }
}