#include "invIndexHeader.h"
#include <stdio.h>


int main() {
    string pathName;
    BplusTree InvIndex;

    printf("Task 2: Build an inverted Index:\n");
    printf("Please input the path of the documents:\nPath: ");
    scanf("%s", pathName);

    InvIndex = InvertedIndex(pathName);
    PrintBPTree(InvIndex);

    return 0;
}