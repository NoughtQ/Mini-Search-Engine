#include "invIndexHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// void InvertedIndexUnix(BplusTree T, string pathName);

void InvertedIndexUnix(BplusTree T, string pathName) {

    DIR * dir;
    struct dirent *ent;
    FILE * fp;

    int docCnt = 0;
    int i, j, k;
    
    string term;
    NodeBP tmp;
    BplusTree InvertedIndex = CreateBP();

    dir = opendir(pathName);
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
                strcpy(docNames[docCnt], ent->d_name);
                fp = fopen(docNames[docCnt], "r");
                if (!fp) {
                    perror("Error opening file");
                    exit(1);
                }
                GenerateInvertedIndex(T, docCnt++, fp);
        } 

        fclose(fp);
        closedir(dir);

        PrintBPTree(InvertedIndex);
    } else {
        perror("Could not open directory");
    }
}