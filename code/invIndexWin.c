#include "invIndexHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// void InvertedIndexWin(BplusTree T);

void InvertedIndexWin(BplusTree T) {
    WIN32_FIND_DATA file_data;
    HANDLE hFind = FileFirstFile("docs\\*", &file_data);
    FILE * fp;

    int docCnt = 0;
    int i, j, k;
    
    string term;
    NodeBP tmp;
    BplusTree InvertedIndex = CreateBP();

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                strcpy(docNames[docCnt], file_data.cFileName);
                fp = fopen(docNames[docCnt], "r");
                if (!fp) {
                    perror("Error opening file");
                    return -1;
                }
                HandleInvertedIndex(T, docCnt++, fp);
            }
        } while (FindNextFile(hFind, &file_data) != 0);

        fclose(fp);
        FindClose(hFind);

        PrintBPTree(InvertedIndex);
    } else {
        perror("Could not open directory");
    }
    
    // scanf("%d", &n);
    
    // for (i = 0; i < n; i++) {

    //     scanf("%d", &term);

    //     tmp = Find(term, InvertedIndex);
    //     Insert(term, tmp, InvertedIndex);
    // }

    
    
    return 0;
}