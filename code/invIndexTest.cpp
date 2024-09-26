#include "invIndexHeader.h"
#include <stdio.h>


int main() {
    BplusTree InvIndex;

    InvIndex = InvertedIndex();
    PrintBPTree(InvIndex);

    return 0;
}