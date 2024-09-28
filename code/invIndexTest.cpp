#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

int main(int argc, char * argv[]) {
    int i;
    bool isTest = false;
    bool Print = false;
    bool isFound;
    bool containStopWords = false;
    bool timeRecord = false;
    int findCnt = 0;
    char * pos;
    char tmp[MAXWORDLEN];
    char * word;
    double duration;
    std::wstring word_wstr;
    stemming::english_stem<> StemEnglish;
    BplusTree InvIndex;
    clock_t start, end, tick;

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--test") || !strcmp(argv[i], "-t")) {
            isTest = true;
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
        } else if (!strcmp(argv[i], "--print") || !strcmp(argv[i], "-p")) {
            Print = true;
        } else if (!strcmp(argv[i], "--stopwords") || !strcmp(argv[i], "-sw")) {
            containStopWords = true;
        } else if (!strcmp(argv[i], "--time") || !strcmp(argv[i], "-tr")) {
            timeRecord = true;
        } else {
            printf("Wrong Parameter!\n");
            exit(1);
        }
    }

    if (!timeRecord) {
        InvIndex = InvertedIndex(isTest, containStopWords);
    } else {
        start = clock();
        InvIndex = InvertedIndex(isTest, containStopWords);
        end = clock();
        PrintTime(start, end);
    }
    

    if (Print) {
        PrintBPTree(InvIndex);
    }

    if (InvIndex->size && findCnt) {
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

            if (!timeRecord) {
                FindBP(word, -1, InvIndex, &isFound, findCnt);
            } else {
                start = clock();
                FindBP(word, -1, InvIndex, &isFound, findCnt);
                end = clock();
                PrintTime(start, end);              
            }
            
            if (!isFound) {
                printf("Sorry, no such word in the inverted index!\n");
                printf("-----------------------------------\n");
            }
        }
    }

    return 0;
}