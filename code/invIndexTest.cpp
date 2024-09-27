#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


int main(int argc, char * argv[]) {
    int i;
    bool isTest = false;
    bool noPrint = false;
    bool isFound;
    int findCnt = 0;
    char * pos;
    char tmp[MAXWORDLEN];
    char * word;
    std::wstring word_wstr;
    stemming::english_stem<> StemEnglish;
    BplusTree InvIndex;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--test") || !strcmp(argv[i], "-t")) {
            isTest = true;
            InvIndex = InvertedIndex(TESTMODE);
        } else if (strstr(argv[i], "--find") || strstr(argv[i], "-f")) {
            if ((pos = strchr(argv[i], '='))) {
                findCnt = atoi(pos + 1);
                if (!findCnt) {
                    printf("Wrong Number!\n");
                    exit(1);
                }
            } else {
                findCnt = 1;
            }
        } else if (!strcmp(argv[i], "--noprint") || !strcmp(argv[i], "-np")) {
            noPrint = true;
        } else {
            printf("Wrong Parameter!\n");
            exit(1);
        }

    }
    if (!isTest) {
        InvIndex = InvertedIndex();
    }

    if (!noPrint) {
        PrintBPTree(InvIndex);
    }

    if (findCnt) {
        word = (char *)malloc(sizeof(char) * MAXWORDLEN);
        printf("\nFinding Words Mode(only supports single word finding):\n");
        for (i = 0; i < findCnt; i++) {
            isFound = false;
            printf("Find %d: ", i + 1);
            scanf("%s", tmp);

            strcpy(word, tmp);
            word_wstr = chararrToWstring(word);
            StemEnglish(word_wstr);
            word = wstringToChararr(word_wstr);

            FindBP(word, -1, InvIndex, &isFound, findCnt);
            if (!isFound) {
                printf("Sorry, no such word here!\n");
                printf("-----------------------------------\n");
            }
        }
    }

    return 0;
}