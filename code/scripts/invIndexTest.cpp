#include "invIndexHeader.h"
#include "wordStem/english_stem.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>

int main(int argc, char * argv[]) {        // Use command line parameters
    int i;
    bool isTest = false;                   // Whether open test mode(-t or --test)
    bool Print = false;                    // Whether open print mode(-p or --print)
    bool isFound;                          // Whether the term is found in the inverted index
    bool containStopWords = false;         // Whether open stopword mode(-s or --stopwords)
    bool timeRecord = false;               // Whether open time record mode(-tr or --time)
    int findCnt = 0;                       // The time of finding a single word for find mode(-f=n or --find=n)
    char * pos;                            // Get the number after `=` in parameters
    char tmp[MAXWORDLEN];                  // Store the input string temporarily
    char * word;                           // The word to be searched
    double duration;                       // Duration of running a function
    std::wstring word_wstr;                // wstring form of word
    stemming::english_stem<> StemEnglish;  // Word stemming function
    BplusTree InvIndex;                    // Inverted Index
    clock_t start, end, tick;              // Record the start and the end of the clock

    for (i = 1; i < argc; i++) {           // Read the parameters
        if (!strcmp(argv[i], "--test") || !strcmp(argv[i], "-t")) {  // Test mode
            isTest = true;
        } else if (strstr(argv[i], "--find") || strstr(argv[i], "-f")) {  // Find mode
            if ((pos = strchr(argv[i], '='))) {
                findCnt = atoi(pos + 1);   // Get the number behind `=`
                if (!findCnt) {
                    printf("Wrong Number!\n");
                    exit(1);
                }
            } else {
                findCnt = 1;   // Use default number
            }
        } else if (!strcmp(argv[i], "--print") || !strcmp(argv[i], "-p")) {  // Print mode
            Print = true; 
        } else if (!strcmp(argv[i], "--stopwords") || !strcmp(argv[i], "-s")) {  // Stopword mode
            containStopWords = true;
        } else if (!strcmp(argv[i], "--time") || !strcmp(argv[i], "-tr")) {  // Time record mode
            timeRecord = true;
        } else {   // Error
            printf("Wrong Parameter!\n");
            exit(1);
        }
    }

    if (!timeRecord) {   // No time record
        InvIndex = InvertedIndex(isTest, containStopWords);
    } else {  // Time record
        start = clock();
        InvIndex = InvertedIndex(isTest, containStopWords);
        end = clock();
        PrintTime(start, end);
    }
    
    if (Print) {  // Print the B+ tree
        PrintBPTree(InvIndex);
    }

    if (InvIndex->size && findCnt) {  // Search the single word
        word = (char *)malloc(sizeof(char) * MAXWORDLEN);
        printf("\nFinding Words Mode(only supports single word finding):\n");
        for (i = 0; i < findCnt; i++) {  // For every search
            isFound = false;
            printf("Find %d: ", i + 1);
            scanf("%s", tmp);
            strcpy(word, tmp);

            // Word stemming
            WordStem(word);

            // Find the word
            if (!timeRecord) {  // No time record
                FindBP(word, -1, InvIndex, &isFound, findCnt);
            } else {  // Time record
                start = clock();
                FindBP(word, -1, InvIndex, &isFound, findCnt);
                end = clock();
                PrintTime(start, end);              
            }
            
            // If not found, then give relevant information
            if (!isFound) {
                printf("Sorry, no such word in the inverted index!\n");
                printf("-----------------------------------\n");
            }
        }
    }

    return 0;
}