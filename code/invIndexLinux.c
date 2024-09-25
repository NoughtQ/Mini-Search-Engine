#include "invIndexHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    int n;
    int i, j, k;

    string x;
    NodeBP tmp;
    BplusTree InvertIndex = Create();
    
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {

        scanf("%d", &x);

        tmp = Find(x, InvertIndex);
        Insert(x, tmp, InvertIndex);
    }

    PrintBPTree(InvertIndex);
    
    return 0;
}