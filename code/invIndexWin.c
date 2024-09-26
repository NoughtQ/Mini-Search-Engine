#include "invIndexHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// void InvertedIndexWin(BplusTree T, string pathName);

void InvertedIndexWin(BplusTree T, string pathName) {
    WIN32_FIND_DATA file_data;
    HANDLE hFind = FileFirstFile(pathName, &file_data);
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
                    exit(1);
                }
                GenerateInvertedIndex(T, docCnt++, fp);
            }
        } while (FindNextFile(hFind, &file_data) != 0);

        fclose(fp);
        FindClose(hFind);

        PrintBPTree(InvertedIndex);
    } else {
        perror("Could not open directory");
    }
}